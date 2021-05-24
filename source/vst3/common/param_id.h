// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/audio_modules/types.h"
#include "ha/plugin_base/core/types.h"
#include "pluginterfaces/vst/vsttypes.h"
#include <assert.h>

namespace ha::plugin_base::vst3 {

//-----------------------------------------------------------------------------
Steinberg::Vst::ParamID build_param_id(Steinberg::Vst::UnitID unit_id,
                                       audio_modules::tag_type param_tag);

audio_modules::tag_type extract_param_tag(Steinberg::Vst::ParamID param_id);

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base::vst3