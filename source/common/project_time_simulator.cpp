// Copyright(c) 2016 René Hansen.

#include "ha/plugin_base/common/project_time_simulator.h"

namespace ha::plugin_base {
namespace {

//-----------------------------------------------------------------------------
void recalc_samples_per_beat_inverted(project_time_simulator::context& cx)
{
    cx.samples_per_beat_inverted = cx.tempo / (real(60.) * cx.sample_rate);
}

//-----------------------------------------------------------------------------
void update_project_time_music(project_time_simulator::context& cx,
                               real project_time_music)
{
    // When the transports project_time_music from host does not change
    // we assume that transport has stopped.
    if (cx.project_time_music == project_time_music)
        return;

    cx.project_time_music     = project_time_music;
    cx.simulated_project_time = project_time_music;
}

//-----------------------------------------------------------------------------
void update_tempo(project_time_simulator::context& cx, real tempo)
{
    if (tempo == cx.tempo)
        return;

    cx.tempo = tempo;
    recalc_samples_per_beat_inverted(cx);
}

//-----------------------------------------------------------------------------
} // namespace

//-----------------------------------------------------------------------------
static project_time_simulator::context create()
{
    project_time_simulator::context cx;
    recalc_samples_per_beat_inverted(cx);
    return cx;
}

//-----------------------------------------------------------------------------
void project_time_simulator::set_sample_rate(context& cx, real sample_rate)
{
    cx.sample_rate = sample_rate;
    recalc_samples_per_beat_inverted(cx);
}

//-----------------------------------------------------------------------------
void project_time_simulator::update(context& cx,
                                    double project_time_music,
                                    double tempo)
{
    update_project_time_music(cx, static_cast<real>(project_time_music));
    update_tempo(cx, static_cast<real>(tempo));
}

//-----------------------------------------------------------------------------
real project_time_simulator::get_project_time_music(context const& cx)
{
    return cx.simulated_project_time;
}

//-----------------------------------------------------------------------------
void project_time_simulator::increment(context& cx, i32 block_size)
{
    cx.simulated_project_time +=
        static_cast<real>(block_size) * cx.samples_per_beat_inverted;
}

//-----------------------------------------------------------------------------
void project_time_simulator::reset(context& cx)
{
    cx.simulated_project_time = 0;
}

//-----------------------------------------------------------------------------

} // namespace ha::plugin_base