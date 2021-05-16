// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/audio_modules/module.h"
#include "ha/plugin_base/common/project_time_simulator.h"
#include "ha/plugin_base/core/types.h"
#include "kompositum/component.h"

namespace ha {
namespace plugin_base {
namespace common {

//-----------------------------------------------------------------------------
/**
 * context
 */
struct context
{
    audio_modules_map m_audio_modules;
    audio_modules::process_data m_process_data;
    Kompositum::ComponentPtr m_component;
    project_time_simulator::context project_time_cx;
};

struct audio_busses
{
    audio_modules::audio_busses inputs;
    audio_modules::audio_busses outputs;
};

void add_param_change(context& cx,
                      audio_modules::tag_type pin_tag,
                      audio_modules::real value);

void setup_context(context& cx,
                   entity_component_def const& component_def,
                   parent_child_tree_def const& tree_def);

void setup_processing(context& cx, audio_modules::process_setup& setup);
bool process_audio(context& cx, audio_busses& host_buffers, i32 numSamples);

//-----------------------------------------------------------------------------
} // namespace common
} // namespace plugin_base
} // namespace ha