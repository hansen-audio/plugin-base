// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "hao/audio_modules/module.h"
#include "ha/plugin_base/common/project_time_simulator.h"
#include "ha/plugin_base/core/types.h"
#include "kompositum/component.h"

namespace ha::plugin_base::common {

//-----------------------------------------------------------------------------
/**
 * context
 */
struct context
{
    audio_modules_map audio_modules;
    audio_modules::ProcessData process_data;
    Kompositum::ComponentPtr component;
    project_time_simulator::context project_time_cx;
};

struct AudioBusses
{
    audio_modules::AudioBusses inputs;
    audio_modules::AudioBusses outputs;
};

void add_param_change(context& cx,
                      audio_modules::tag_type pin_tag,
                      audio_modules::real value);

void setup_context(context& cx,
                   entity_component_def const& component_def,
                   parent_child_tree_def const& tree_def);

void setup_processing(context& cx, audio_modules::ProcessSetup& setup);
bool process_audio(context& cx, AudioBusses& host_buffers, i32 numSamples);

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base::common
