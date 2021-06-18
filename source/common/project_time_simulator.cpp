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
f64 project_time_simulator::synchronise(context const& cx,
                                        f64 project_time_music)
{
    return project_time_music;
}

//-----------------------------------------------------------------------------
f64 project_time_simulator::advance(context const& cx,
                                    f64 project_time_music,
                                    i32 block_size)
{

    f64 delta = static_cast<f64>(block_size) * cx.samples_per_beat_inverted;
    return project_time_music + delta;
}

//-----------------------------------------------------------------------------
void project_time_simulator::update_tempo(context& cx, real value)
{
    if (value == cx.tempo)
        return;

    cx.tempo = value;
    recalc_samples_per_beat_inverted(cx);
}

//-----------------------------------------------------------------------------

} // namespace ha::plugin_base