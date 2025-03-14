// Copyright(c) 2021 Hansen Audio.

#pragma once

#include <cstdint>
#include <map>
#include <memory>

namespace hao {

//-----------------------------------------------------------------------------
namespace audio_modules {
enum class ModuleTags;
class ModuleImpl;
} // namespace audio_modules

namespace plugin_base {
//-----------------------------------------------------------------------------
using real                  = float const;
using mut_real              = std::remove_const<real>::type;
using i32                   = std::int32_t;
using f64                   = double;
using IDType                = std::uint64_t;
using parent_child_tree_def = std::multimap<IDType, IDType>;
using entity_component_def  = std::map<IDType, audio_modules::ModuleTags>;
using audio_module_ptr      = std::unique_ptr<audio_modules::ModuleImpl>;
using audio_modules_map     = std::map<IDType, audio_module_ptr>;

//-----------------------------------------------------------------------------
} // namespace plugin_base
} // namespace hao