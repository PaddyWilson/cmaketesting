cmake_minimum_required(VERSION 3.28)
include (FetchContent)
include (ExternalProject)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        master
)
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        master
)

# FetchContent_Declare(
#     rlimgui
#     GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
#     GIT_TAG        main
# )
# FetchContent_MakeAvailable(rlimgui)
ExternalProject_Add(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG        main

    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgiu
    CONFIGURE_COMMAND "cd ${CMAKE_CURRENT_SOURCE_DIR}/rlimgiu && ./premake5 gmake2"
    BUILD_COMMAND "${MAKE}"
)

set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "")
FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    #GIT_SHALLOW    TRUE
    #GIT_TAG        master
    #SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson"
    #BINARY_DIR "${CMAKE_BINARY_DIR}/SDKs/rapidjson"
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
)

FetchContent_Declare(
    enet
    GIT_REPOSITORY https://github.com/zpl-c/enet.git
)

FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG        master
)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(raylib EnTT yaml-cpp json nfd enet rapidjson box2d)

project(Tutorial1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE_FILES "*.cpp" "${enet_SOURCE_DIR}/include" "${rapidjson_SOURCE_DIR}/include")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# include files from ProjectB
#include_directories(${CMAKE_SOURCE_DIR}/src2 ${enet_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PUBLIC "${enet_SOURCE_DIR}/include" "${rapidjson_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} raylib yaml-cpp nfd EnTT::EnTT nlohmann_json::nlohmann_json box2d)

#install(TARGETS ${PROJECT_NAME}
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib)

#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "../build")
