cmake_minimum_required(VERSION 3.14.0)

project(ha-plugin-base)

add_subdirectory(external)

# plugin-base
add_library(plugin-base STATIC
    include/ha/plugin_base/common/audio_module_visitor.h
    include/ha/plugin_base/common/context.h
    include/ha/plugin_base/common/project_time_simulator.h
    include/ha/plugin_base/vst3/component/audio_processing.h
    include/ha/plugin_base/vst3/component/audio_setup.h
    include/ha/plugin_base/vst3/controller/generic_param.h
    source/common/context.cpp
    source/common/project_time_simulator.cpp
    source/vst3/common/param_id.cpp
    source/vst3/common/param_id.h
    source/vst3/component/audio_processing.cpp
    source/vst3/component/audio_setup.cpp
    source/vst3/controller/generic_param.cpp
)

target_include_directories(plugin-base
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source
)

target_link_libraries(plugin-base
    PUBLIC
        aligned-allocator
        audio-modules
        kompositum
        sdk
)

target_compile_features(plugin-base
    PUBLIC 
        cxx_std_11
)

# plugin-base_test
add_executable(plugin-base_test
    test/plugin-base_test.cpp
)

target_link_libraries(plugin-base_test
    PRIVATE
        plugin-base
        gtest
        gtest_main
)

add_test(NAME plugin-base_test 
    COMMAND plugin-base_test
)

enable_testing()