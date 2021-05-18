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
     * @brief Updates tempo and project time. Only call this when transport is
     * running.
     *
     * @param project_time_music Project time of the host in quarter notes
     * @param tempo Tempo of the host
     * @return Returns the project time
     */
    static real update(context& cx, double project_time_music, double tempo);

    /**
     * @brief
     *
     * @param project_time_music Current internal project time to be advanced.
     * @param block_size The number of samples to advance.
     * @return Returns the advanced project time.
     */
    static real advance(context& cx, real project_time_music, i32 block_size);

    static void set_sample_rate(context& cx, real sample_rate);
};

//-----------------------------------------------------------------------------
} // namespace ha::plugin_base