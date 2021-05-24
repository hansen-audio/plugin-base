// Copyright(c) 2021 Hansen Audio.

#include "ha/plugin_base/common/context.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"

namespace ha::plugin_base::vst3 {

//-----------------------------------------------------------------------------
bool setup_processing(common::context& context,
                      Steinberg::Vst::ProcessSetup& processSetup,
                      Steinberg::Vst::IComponent* component);

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base::vst3