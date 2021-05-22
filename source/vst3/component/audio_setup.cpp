// Copyright(c) 2021 Hansen Audio.

#include "ha/plugin_base/vst3/component/audio_setup.h"
//#include "pluginterfaces/vst/ivstprocesscontext.h"

namespace ha {
namespace plugin_base {
namespace vst3 {

//-----------------------------------------------------------------------------
audio_modules::audio_busses init_busses(Steinberg::Vst::IComponent* component,
                                        Steinberg::Vst::BusDirection dir,
                                        Steinberg::int32 maxBlockSize)
{
    audio_modules::audio_busses new_busses;

    for (Steinberg::int32 busIdx = 0;
         busIdx <
         component->getBusCount(Steinberg::Vst::MediaTypes::kAudio, dir);
         ++busIdx)
    {
        audio_modules::audio_bus local_bus;
        Steinberg::Vst::BusInfo bus_info;
        if (component->getBusInfo(Steinberg::Vst::MediaTypes::kAudio, dir,
                                  busIdx, bus_info) != Steinberg::kResultOk)
            continue;

        local_bus.resize(bus_info.channelCount);
        for (auto& channel : local_bus)
            channel.resize(maxBlockSize);

        new_busses.emplace_back(local_bus);
    }

    return new_busses;
}

//-----------------------------------------------------------------------------
bool setup_audio_busses(common::context& context,
                        Steinberg::Vst::IComponent* component,
                        Steinberg::int32 max_block_size)
{
    if (!component)
        return false;

    context.process_data.inputs = init_busses(
        component, Steinberg::Vst::BusDirections::kInput, max_block_size);
    context.process_data.outputs = init_busses(
        component, Steinberg::Vst::BusDirections::kOutput, max_block_size);

    return true;
}

//-----------------------------------------------------------------------------
audio_modules::process_setup
create_process_setup(Steinberg::Vst::ProcessSetup const& setup)
{
    audio_modules::process_setup ag_setup{
        /*sample_rate   =*/audio_modules::real(setup.sampleRate),
        /*block_size    =*/setup.maxSamplesPerBlock,
        /*reciprocal_sr =*/audio_modules::real(1. / ag_setup.sample_rate)};
    return ag_setup;
}

//-----------------------------------------------------------------------------
bool setup_processing(common::context& context,
                      Steinberg::Vst::ProcessSetup& processSetup)
{
    audio_modules::process_setup ag_setup =
        plugin_base::vst3::create_process_setup(processSetup);
    common::setup_processing(context, ag_setup);

    return true;
}

//-----------------------------------------------------------------------------
bool setup_processing(common::context& context,
                      Steinberg::Vst::ProcessSetup& processSetup,
                      Steinberg::Vst::IComponent* component)
{
    constexpr Steinberg::int32 max_num_samples_internal = 64;
    setup_audio_busses(context, component, max_num_samples_internal);
    vst3::setup_processing(context, processSetup);

    return true;
}

//-----------------------------------------------------------------------------
} // namespace vst3
} // namespace plugin_base
} // namespace ha