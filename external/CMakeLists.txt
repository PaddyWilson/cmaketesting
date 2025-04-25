cmake_minimum_required(VERSION 3.28)
include(FetchContent)
include(ExternalProject)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.12.2
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "")
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

# FetchContent_Declare(
#     rlimgui
#     GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
#     GIT_TAG        main
# )
# FetchContent_MakeAvailable(rlimgui)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set (rlPATCH_COMMAND "")
    set (rlCONFIGURE_COMMAND premake5.exe vs2022)
    set (rlBUILD_COMMAND premake5.exe vs2022)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (rlPATCH_COMMAND chmod +x premake5)
    set (rlCONFIGURE_COMMAND ./premake5 gmake2)
    set (rlBUILD_COMMAND make)
ENDIF()

ExternalProject_Add(
    rlimguiDownload
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
    GIT_SHALLOW true
    
    LOG_CONFIGURE true
    LOG_BUILD true
    #BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui
    #TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui
    #LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/src/rlimgui
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/src/rlimgui
    #STAMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui
    INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/src/rlimgui/bin/Debug

    PATCH_COMMAND ${rlPATCH_COMMAND}
    CONFIGURE_COMMAND ${rlCONFIGURE_COMMAND}
    BUILD_COMMAND ${rlBUILD_COMMAND}
    INSTALL_COMMAND ""
)
SET(rlimgui_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/src/rlimgui CACHE INTERNAL "")
SET(rlimgui_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/src/rlimgui/bin/Debug CACHE INTERNAL "")

# add_library(rlimgui STATIC IMPORTED)
# set_target_properties(rlimgui PROPERTIES IMPORTED_LOCATION ${rlimgui_LIB_DIR}/librlImGui.a)

set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "")
FetchContent_Declare(
    RapidJSON
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    #GIT_TAG        master
    GIT_SHALLOW true
    #SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson"
    #BINARY_DIR "${CMAKE_BINARY_DIR}/SDKs/rapidjson"
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

set(ENET_TEST off CACHE BOOL "")
FetchContent_Declare(
    enet
    GIT_REPOSITORY https://github.com/zpl-c/enet.git
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG master
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG main
    GIT_SHALLOW true
    OVERRIDE_FIND_PACKAGE
)

ExternalProject_Add(
    collishiDownload
    GIT_REPOSITORY https://github.com/Shidacea/Collishi.git
    GIT_TAG master
    GIT_SHALLOW true
    LOG_CONFIGURE true
    LOG_BUILD true
    #BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/collishi
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/collishi/src/collishi
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/collishi/src/collishi
    INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/collishi/src/collishi/bin/Debug

    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

SET(collishi_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/collishi/src/collishi CACHE INTERNAL "")
# SET(collishi_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/collishi/src/collishi/bin/Debug CACHE INTERNAL "")

ExternalProject_Add(
    cute_headersDownload
    GIT_REPOSITORY https://github.com/RandyGaul/cute_headers.git
    GIT_TAG master
    GIT_SHALLOW true

    LOG_CONFIGURE true
    LOG_BUILD true
    #BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/cute_headers
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cute_headers/src/cute_headers
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cute_headers/src/cute_headers
    INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cute_headers/src/cute_headers/bin/Debug

    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

SET(cute_headers_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cute_headers/src/cute_headers CACHE INTERNAL "")

ExternalProject_Add(
    physacDownload
    GIT_REPOSITORY https://github.com/victorfisac/Physac.git
    GIT_TAG master
    GIT_SHALLOW true
    LOG_CONFIGURE true
    LOG_BUILD true
    #BUILD_IN_SOURCE 1
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/physac
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physac/src/physac

    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

SET(physac_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physac/src/physac/src CACHE INTERNAL "")

set(BUILD_CURL_EXE off CACHE BOOL "")
set(BUILD_EXAMPLES off CACHE BOOL "")
set(BUILD_LIBCURL_DOCS off CACHE BOOL "")
set(BUILD_MISC_DOCS off CACHE BOOL "")
set(BUILD_TESTING off CACHE BOOL "")

set(BUILD_STATIC_LIBS off CACHE BOOL "")
set(CURL_ENABLE_SSL on CACHE BOOL "")
FetchContent_Declare(
    curl
    URL https://curl.se/download/curl-8.11.0.tar.gz
    # GIT_TAG main
    GIT_SHALLOW true
    DOWNLOAD_EXTRACT_TIMESTAMP true
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(raylib EnTT yaml-cpp nlohmann_json nfd enet RapidJSON box2d curl)
