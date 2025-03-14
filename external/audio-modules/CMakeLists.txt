cmake_minimum_required(VERSION 3.15.0)

include(FetchContent)

FetchContent_Declare(
    audio-modules
    GIT_REPOSITORY https://github.com/hansen-audio/audio-modules.git
    GIT_TAG main
)

FetchContent_MakeAvailable(audio-modules)