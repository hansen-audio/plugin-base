// Copyright(c) 2016 René Hansen.

#pragma once

#include "ha/plugin_base/core/types.h"

namespace ha::plugin_base {

/**
 * @brief project_time_simulator
 *
 */
struct project_time_simulator
{
    struct context
    {
        mut_real m_sample_rate               = 44100.f;
        mut_real m_simulated_project_time    = 0.f;
        mut_real m_samples_per_beat_inverted = 0.f;
        mut_real m_tempo                     = 120.f;
        mut_real m_project_time_music        = 0.0;
    };

    static context create();

    static void update(context& cx, double project_time_music, double tempo);
    static real get_project_time_music(context const& cx);
    static void increment(context& cx, i32 block_size);
    static void set_sample_rate(context& cx, real sample_rate);
    static void reset(context& cx);
};

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base