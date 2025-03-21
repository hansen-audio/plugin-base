// Copyright(c) 2016 René Hansen.

#pragma once

#include "hao/plugin_base/core/types.h"

namespace hao::plugin_base {

/**
 * @brief project_time_simulator
 *
 */
struct project_time_simulator
{
    struct context
    {
        mut_real sample_rate               = 44100.f;
        mut_real samples_per_beat_inverted = 0.f;
        mut_real tempo                     = 120.f;
    };

    /**
     * @brief Creates a usable context
     *
     * @return context
     */
    static context create();

    /**
     * @brief Synchronises to tempo and project time of the host. Only call this
     * when transport is running.
     *
     * @param project_time_music Project time of the host in quarter notes
     * @return Returns the project time
     */
    static f64 synchronise(context const& cx, f64 project_time_music);

    /**
     * @brief
     *
     * @param project_time_music Current internal project time to be advanced.
     * @param block_size The number of samples to advance.
     * @return Returns the advanced project time.
     */
    static f64
    advance(context const& cx, f64 project_time_music, i32 block_size);

    static void update_tempo(context& cx, real value);
    static void set_sample_rate(context& cx, real sample_rate);
};

//-----------------------------------------------------------------------------
} // namespace hao::plugin_base