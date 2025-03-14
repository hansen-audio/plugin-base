// Copyright(c) 2021 Hansen Audio.

#include "hao/plugin_base/common/context.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"

namespace hao::plugin_base::vst3 {

//-----------------------------------------------------------------------------
bool setup_processing(common::context& context,
                      Steinberg::Vst::ProcessSetup& processSetup,
                      Steinberg::Vst::IComponent* component);

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::vst3