// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "hao/audio_modules/types.h"
#include "hao/plugin_base/core/types.h"
#include "pluginterfaces/vst/vsttypes.h"
#include <assert.h>

namespace hao::plugin_base::vst3 {

//-----------------------------------------------------------------------------
Steinberg::Vst::ParamID build_param_id(Steinberg::Vst::UnitID unit_id,
                                       audio_modules::tag_type param_tag);

audio_modules::tag_type extract_param_tag(Steinberg::Vst::ParamID param_id);

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::vst3