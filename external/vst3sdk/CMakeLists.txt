cmake_minimum_required(VERSION 3.14.0)

include(FetchContent)

FetchContent_Declare(
    vst3sdk
    # GIT_REPOSITORY ${HA_GIT_REPOSITORY_URL}/steinbergmedia/vst3sdk.git
    GIT_REPOSITORY https://www.github.com/steinbergmedia/vst3sdk.git
    GIT_TAG v3.7.8_build_34
)

# Disable some VST 3 SDK stuff
option(SMTG_RUN_VST_VALIDATOR OFF)
option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

if(WIN32)
    option(SMTG_CREATE_PLUGIN_LINK OFF)
endif()

FetchContent_MakeAvailable(vst3sdk)

# Solve problem with public_sdk_SOURCE_DIR not being defined when using smtg_enable_vst3_sdk().
# set(vst3sdk_SOURCE_DIR ${vst3sdk_SOURCE_DIR} CACHE INTERNAL "VST 3 SDK source directory.") 
