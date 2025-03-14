// Copyright(c) 2021 Hansen Audio.

#include "hao/plugin_base/common/context.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"

namespace ha::plugin_base::vst3 {

//-----------------------------------------------------------------------------
bool process_audio(common::context& context,
                   Steinberg::Vst::ProcessData& processData);

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base::vst3