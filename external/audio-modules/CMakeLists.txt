cmake_minimum_required(VERSION 3.15.0)

include(FetchContent)

FetchContent_Declare(
    audio-modules
    GIT_REPOSITORY ${HA_GIT_REPOSITORY_URL}/hansen-audio/audio-modules.git
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(audio-modules)