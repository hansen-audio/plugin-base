// Copyright(c) 2021 Hansen Audio.

#include "vst3/common/param_id.h"
#include "hao/audio_modules/types.h"
#include "hao/plugin_base/core/types.h"
#include <assert.h>

namespace hao::plugin_base::vst3 {

//-----------------------------------------------------------------------------
constexpr i32 MASK      = 0xFFFF;
constexpr i32 BIT_SHIFT = 16;

//-----------------------------------------------------------------------------
Steinberg::Vst::ParamID build_param_id(Steinberg::Vst::UnitID unit_id,
                                       audio_modules::tag_type param_tag)
{
    unit_id &= MASK;
    audio_modules::tag_type masked_param_tag = param_tag & MASK;
    return (unit_id << BIT_SHIFT) | masked_param_tag;
}

//-----------------------------------------------------------------------------
audio_modules::tag_type extract_param_tag(Steinberg::Vst::ParamID param_id)
{
    return param_id & MASK;
}

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::vst3