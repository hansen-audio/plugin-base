// Copyright(c) 2021 Hansen Audio.

#include "hao/plugin_base/common/context.h"
#include "hao/audio_modules/module_factory.h"
#include "hao/plugin_base/common/audio_module_visitor.h"
#include "kompositum/builder.h"
#include "slicing.h"

namespace hao::plugin_base::common {

//-----------------------------------------------------------------------------
void add_param_change(common::context& cx,
                      audio_modules::tag_type pin_tag,
                      audio_modules::real value)
{
    cx.process_data.param_inputs.push_back({pin_tag, value});
}

//-----------------------------------------------------------------------------
static void create_audio_modules_map(common::context& cx,
                                     entity_component_def const& def)
{
    audio_modules_map nodes_map;
    for (auto const& element : def)
    {
        auto new_node = audio_modules::ModuleFactoryImpl::create_audio_module(
            element.second);
        nodes_map.emplace(element.first, std::move(new_node));
    }

    cx.audio_modules = std::move(nodes_map);
}

//-----------------------------------------------------------------------------
void setup_processing(common::context& cx, audio_modules::ProcessSetup& setup)
{
    common::audio_module_visitor process_setup_visitor(
        [&](Kompositum::IDType uid, bool is_composite) {
            auto item = cx.audio_modules.find(uid);
            if (item == cx.audio_modules.end())
                return;

            item->second->setup_processing(setup);
        });

    cx.component->accept(process_setup_visitor);
    cx.process_data.sample_rate = setup.sample_rate;
    project_time_simulator::set_sample_rate(cx.project_time_cx,
                                            setup.sample_rate);
}

//-----------------------------------------------------------------------------
void setup_context(common::context& cx,
                   entity_component_def const& component_def,
                   parent_child_tree_def const& tree_def)
{
    create_audio_modules_map(cx, component_def);
    cx.component = Kompositum::buildComposite(0, tree_def);
    cx.process_data.param_inputs.reserve(256);  // TODO
    cx.process_data.param_outputs.reserve(256); // TODO
}

//------------------------------------------------------------------------
bool process_audio(common::context& cx,
                   common::AudioBusses& host_buffers,
                   i32 numSamples)
{
    common::slice(numSamples, [&](i32 begin, i32 num) {
        common::audio_module_visitor process_audio_visitor(
            [&](Kompositum::IDType uid, bool is_composite) {
                auto item = cx.audio_modules.find(uid);
                if (item == cx.audio_modules.end())
                    return;

                cx.process_data.num_samples = num;
                auto& proc_inputs           = cx.process_data.inputs;
                auto& proc_outputs          = cx.process_data.outputs;
                for (size_t bi = 0; bi < proc_inputs.size(); ++bi)
                {
                    for (size_t ci = 0; ci < proc_inputs[bi].size(); ++ci)
                    {
                        proc_inputs[bi][ci].resize(cx.process_data.num_samples);
                        proc_outputs[bi][ci].resize(
                            cx.process_data.num_samples);

                        auto& vec0       = proc_inputs[bi][ci];
                        auto const& vec1 = host_buffers.inputs[bi][ci];
                        vec0.assign(vec1.begin() + begin,
                                    vec1.begin() + begin + num);
                    }
                }

                if (item->second)
                    item->second->process_audio(cx.process_data);

                for (size_t bi = 0; bi < proc_outputs.size(); ++bi)
                {
                    for (size_t ci = 0; ci < proc_outputs[bi].size(); ++ci)
                    {
                        auto const& vec0 = proc_outputs[bi][ci];
                        auto& vec1       = host_buffers.outputs[bi][ci];
                        std::copy(vec0.begin(), vec0.end(),
                                  vec1.begin() + begin);
                    }
                }
            });

        cx.component->accept(process_audio_visitor);
    });

    return true;
}
//-----------------------------------------------------------------------------
} // namespace hao::plugin_base::common