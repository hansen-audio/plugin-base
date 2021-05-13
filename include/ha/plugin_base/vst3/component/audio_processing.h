// Copyright(c) 2021 Hansen Audio.

#include "ha/plugin_base/common/context.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"

namespace ha {
namespace plugin_base {
namespace vst3 {

//-----------------------------------------------------------------------------
bool process_audio(common::context& context, Steinberg::Vst::ProcessData& processData);

//-----------------------------------------------------------------------------
} // namespace vst3
} // namespace plugin_base
} // namespace ha