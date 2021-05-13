// Copyright(c) 2021 Hansen Audio.

#pragma once

#include <cstdint>
#include <map>
#include <memory>

namespace ha {

//-----------------------------------------------------------------------------
namespace audio_modules {
enum class module_tags;
class module;
} // namespace audio_modules

namespace plugin_base {
//-----------------------------------------------------------------------------
using i32                   = std::int32_t;
using IDType                = std::uint64_t;
using parent_child_tree_def = std::multimap<IDType, IDType>;
using entity_component_def  = std::map<IDType, audio_modules::module_tags>;
using audio_module_ptr      = std::unique_ptr<audio_modules::module>;
using audio_modules_map     = std::map<IDType, audio_module_ptr>;

//-----------------------------------------------------------------------------
} // namespace plugin_base
} // namespace ha