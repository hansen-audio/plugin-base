// Copyright(c) 2021 Hansen Audio.

#include "common/slicing.h"
#include "hao/audio_modules/types.h"
#include "hao/plugin_base/common/audio_module_visitor.h"
#include "hao/plugin_base/common/context.h"
#include "hao/plugin_base/core/types.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "vst3/common/param_id.h"

namespace hao::plugin_base::vst3 {

//-----------------------------------------------------------------------------
bool copy_inputs(i32 begin,
                 i32 numSamples,
                 Steinberg::Vst::ProcessData& processData,
                 common::context& context)
{
    context.process_data.num_samples = numSamples;

    for (std::size_t bi = 0; bi < context.process_data.inputs.size(); ++bi)
    {
        if (processData.inputs)
        {
            auto& bus = context.process_data.inputs[bi];
            if (bi >= std::size_t(processData.numInputs))
                continue;

            Steinberg::Vst::AudioBusBuffers vst_bus = processData.inputs[bi];

            for (std::size_t ci = 0; ci < bus.size(); ++ci)
            {
                auto& channel = bus[ci];
                if (ci >= std::size_t(vst_bus.numChannels))
                    continue;

                Steinberg::Vst::Sample32* vst_channel =
                    vst_bus.channelBuffers32[ci];
                if (!vst_channel)
                    continue;

                channel.resize(numSamples);
                channel.assign(vst_channel + begin,
                               vst_channel + begin + numSamples);
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
bool copy_outputs(i32 begin,
                  i32 numSamples,
                  common::context& context,
                  Steinberg::Vst::ProcessData& processData)
{
    for (std::size_t bi = 0; bi < context.process_data.outputs.size(); ++bi)
    {
        if (processData.outputs)
        {
            auto& bus = context.process_data.outputs[bi];
            if (bi >= std::size_t(processData.numOutputs))
                continue;

            Steinberg::Vst::AudioBusBuffers vst_bus = processData.outputs[bi];

            for (std::size_t ci = 0; ci < bus.size(); ++ci)
            {
                auto& channel = bus[ci];
                if (ci >= std::size_t(vst_bus.numChannels))
                    continue;

                Steinberg::Vst::Sample32* vst_channel =
                    vst_bus.channelBuffers32[ci];
                if (!vst_channel)
                    continue;

                std::copy(channel.begin(), channel.begin() + numSamples,
                          vst_channel + begin);
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
bool copy_process_context(Steinberg::Vst::ProcessData& processData,
                          common::context& context)
{
    Steinberg::Vst::ProcessContext* pcx = processData.processContext;
    if (!pcx)
        return false;

    if (pcx->state & Steinberg::Vst::ProcessContext::kTempoValid)
    {
        project_time_simulator::update_tempo(context.project_time_cx,
                                             pcx->tempo);
    }

    if (pcx->state & Steinberg::Vst::ProcessContext::kContTimeValid)
    {
        context.process_data.continous_time_samples = pcx->continousTimeSamples;
    }

    if (pcx->state & Steinberg::Vst::ProcessContext::kPlaying)
    {
        if (pcx->state & Steinberg::Vst::ProcessContext::kProjectTimeMusicValid)
        {
            f64 spt = project_time_simulator::synchronise(
                context.project_time_cx, pcx->projectTimeMusic);

            context.process_data.project_time_music = spt;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
bool copy_param_inputs(i32 begin,
                       i32 numSamples,
                       Steinberg::Vst::ProcessData& processData,
                       common::context& context)
{
    if (!processData.inputParameterChanges)
        return false;

    context.process_data.param_inputs.clear();

    Steinberg::int32 numParamsChanged =
        processData.inputParameterChanges->getParameterCount();
    for (Steinberg::int32 index = 0; index < numParamsChanged; index++)
    {
        auto* paramQueue =
            processData.inputParameterChanges->getParameterData(index);
        if (!paramQueue)
            continue;

        Steinberg::Vst::ParamValue value = 0.;
        Steinberg::int32 sampleOffset    = 0;
        Steinberg::int32 const numPoints = paramQueue->getPointCount();
        if (numPoints <= 0)
            continue;

        /**
         * TODO: In order to keep it simple for now, just take the last
         * point from the queue. Ignore the sample offset as well.
         */
        real last_point = numPoints - 1;
        if (paramQueue->getPoint(last_point, sampleOffset, value) !=
            Steinberg::kResultOk)
            continue;

        auto const paramId  = paramQueue->getParameterId();
        auto const paramTag = extract_param_tag(paramId);

        context.process_data.param_inputs.push_back(
            {paramTag, static_cast<float>(value)});
    }

    return true;
}

//-----------------------------------------------------------------------------
bool copy_param_outputs(common::context& cx,
                        Steinberg::Vst::ProcessData& processData)
{
    if (cx.process_data.param_outputs.empty())
        return true;

    if (!processData.outputParameterChanges)
        return true;

    /**
     * TODO: Is this a good solution for now? Get out the last param change for
     * each param and push it into the VST 3 param queue.
     */
    while (!cx.process_data.param_outputs.empty())
    {
        auto const last = cx.process_data.param_outputs.back();

        // Remove all other param changes of 'last.tag'.
        for (auto it = cx.process_data.param_outputs.begin();
             it != cx.process_data.param_outputs.end();)
        {
            if ((*it).tag == last.tag)
                it = cx.process_data.param_outputs.erase(it);
            else
                ++it;
        }

        Steinberg::int32 index = -1;
        auto* queue = processData.outputParameterChanges->addParameterData(
            last.tag, index);

        if (!queue)
            continue;

        queue->addPoint(0, last.value, index);
    }

    cx.process_data.param_outputs.clear();

    return true;
}

//-----------------------------------------------------------------------------
void advance_project_time(common::context& cx)
{
    // clang-format off
    cx.process_data.project_time_music = 
        project_time_simulator::advance(
            cx.project_time_cx, 
            cx.process_data.project_time_music,
            cx.process_data.num_samples);
    // clang-format on
}

//-----------------------------------------------------------------------------
bool process(common::context& cx)
{
    common::audio_module_visitor process_audio_visitor(
        [&](Kompositum::IDType uid, bool is_composite) {
            auto item = cx.audio_modules.find(uid);
            if (item == cx.audio_modules.end())
                return;

            if (!item->second)
                return;

            item->second->process_audio(cx.process_data);

            /**
             * Add unit ID to all param changes in order to make the internal
             * tag a VST 3 ParamID.
             */
            for (auto& p : cx.process_data.param_outputs)
            {
                p.tag = build_param_id(uid, p.tag);
            }
        });

    cx.component->accept(process_audio_visitor);
    advance_project_time(cx);

    return true;
} // namespace vst3

//-----------------------------------------------------------------------------
bool process_audio(common::context& context,
                   Steinberg::Vst::ProcessData& processData)
{
    copy_param_inputs(0, 0, processData, context);
    copy_process_context(processData, context);

    common::slice(processData.numSamples, [&](i32 begin, i32 numSamples) {
        copy_inputs(begin, numSamples, processData, context);
        process(context);
        copy_outputs(begin, numSamples, context, processData);
    });

    copy_param_outputs(context, processData);

    return true;
}

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::vst3