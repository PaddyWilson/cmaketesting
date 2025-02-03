cmake_minimum_required(VERSION 3.11)
include(FetchContent)
include(ExternalProject)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

project(root_project)

add_subdirectory(external)

add_subdirectory(src)
add_subdirectory(src2)
add_subdirectory(src3)
add_subdirectory(gamethinger)
add_subdirectory(subgamethinger)
