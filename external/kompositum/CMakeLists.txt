cmake_minimum_required(VERSION 3.15.0)

include(FetchContent)

FetchContent_Declare(
    kompositum
    GIT_REPOSITORY https://github.com/rehans/Kompositum.git
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(kompositum)