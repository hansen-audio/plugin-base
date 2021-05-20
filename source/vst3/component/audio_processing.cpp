// Copyright(c) 2021 Hansen Audio.

#include "common/slicing.h"
#include "ha/audio_modules/types.h"
#include "ha/plugin_base/common/audio_module_visitor.h"
#include "ha/plugin_base/common/context.h"
#include "ha/plugin_base/core/types.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "vst3/common/param_id.h"

namespace ha {
namespace plugin_base {
namespace vst3 {

//-----------------------------------------------------------------------------
bool copy_inputs(i32 begin,
                 i32 numSamples,
                 Steinberg::Vst::ProcessData& processData,
                 common::context& context)
{
    context.m_process_data.num_samples = numSamples;

    for (i32 bi = 0; bi < context.m_process_data.inputs.size(); ++bi)
    {
        if (processData.inputs)
        {
            auto& bus = context.m_process_data.inputs[bi];
            if (bi >= processData.numInputs)
                continue;

            Steinberg::Vst::AudioBusBuffers vst_bus = processData.inputs[bi];

            for (i32 ci = 0; ci < bus.size(); ++ci)
            {
                auto& channel = bus[ci];
                if (ci >= vst_bus.numChannels)
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
    for (i32 bi = 0; bi < context.m_process_data.outputs.size(); ++bi)
    {
        if (processData.outputs)
        {
            auto& bus = context.m_process_data.outputs[bi];
            if (bi >= processData.numOutputs)
                continue;

            Steinberg::Vst::AudioBusBuffers vst_bus = processData.outputs[bi];

            for (i32 ci = 0; ci < bus.size(); ++ci)
            {
                auto& channel = bus[ci];
                if (ci >= vst_bus.numChannels)
                    continue;

                Steinberg::Vst::Sample32* vst_channel =
                    vst_bus.channelBuffers32[ci];
                if (!vst_channel)
                    continue;

                channel.resize(numSamples);
                std::copy(channel.begin(), channel.end(), vst_channel + begin);
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
        context.m_process_data.continous_time_samples =
            pcx->continousTimeSamples;
    }

    if ((pcx->state & (Steinberg::Vst::ProcessContext::kPlaying |
                       Steinberg::Vst::ProcessContext::kProjectTimeMusicValid)))
    {
        real spt = project_time_simulator::synchronise(context.project_time_cx,
                                                       pcx->projectTimeMusic);

        context.m_process_data.project_time_music = spt;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool copy_param_changes(i32 begin,
                        i32 numSamples,
                        Steinberg::Vst::ProcessData& processData,
                        common::context& context)
{
    if (processData.inputParameterChanges)
    {
        context.m_process_data.mod_data.pin_datas.clear();
        Steinberg::int32 numParamsChanged =
            processData.inputParameterChanges->getParameterCount();
        for (Steinberg::int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue =
                    processData.inputParameterChanges->getParameterData(index))
            {
                Steinberg::Vst::ParamValue value = 0.;
                Steinberg::int32 sampleOffset    = 0;
                Steinberg::int32 const numPoints = paramQueue->getPointCount();
                if (paramQueue->getPoint(0, sampleOffset, value) !=
                    Steinberg::kResultOk)
                    continue;

                Steinberg::Vst::ParamID const paramId =
                    paramQueue->getParameterId();
                Steinberg::Vst::UnitID const unitId = paramId << 16;
                audio_modules::tag_type const paramTag =
                    extract_param_tag(paramId);
                context.m_process_data.mod_data.pin_datas.push_back(
                    {paramTag, static_cast<float>(value)});
            }
        }

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool process(common::context& context)
{
    common::audio_module_visitor process_audio_visitor(
        [&](Kompositum::IDType uid, bool is_composite) {
            auto item = context.m_audio_modules.find(uid);
            if (item == context.m_audio_modules.end())
                return;

            if (item->second)
                item->second->process_audio(context.m_process_data);
        });

    context.m_component->accept(process_audio_visitor);
    context.m_process_data.project_time_music = project_time_simulator::advance(
        context.project_time_cx, context.m_process_data.project_time_music,
        context.m_process_data.num_samples);

    return true;
}

//-----------------------------------------------------------------------------
bool process_audio(common::context& context,
                   Steinberg::Vst::ProcessData& processData)
{
    copy_param_changes(0, 0, processData, context);
    copy_process_context(processData, context);

    common::slice(processData.numSamples, [&](i32 begin, i32 numSamples) {
        copy_inputs(begin, numSamples, processData, context);
        process(context);
        copy_outputs(begin, numSamples, context, processData);
    });

    return true;
}

//-----------------------------------------------------------------------------
} // namespace vst3
} // namespace plugin_base
} // namespace ha