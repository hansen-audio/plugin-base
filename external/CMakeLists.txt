cmake_minimum_required(VERSION 3.15.0)

if(NOT HA_GIT_REPOSITORY_URL)
    set(HA_GIT_REPOSITORY_URL "https://www.github.com")
endif()

add_subdirectory(audio-modules)
add_subdirectory(aligned-allocator)
add_subdirectory(googletest)
add_subdirectory(kompositum)
add_subdirectory(vst3sdk)
