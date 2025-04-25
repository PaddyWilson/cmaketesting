cmake_minimum_required(VERSION 3.28)
include(FetchContent)
include(ExternalProject)

find_package(box2d)
# find_package(collishi)
find_package(enet)
find_package(EnTT)
find_package(nfd)
find_package(rapidjson)
# find_package(rlimgui) cant use with fetchcontent_add because it does not use cmake
find_package(yaml-cpp)
find_package(nlohmann_json)
# find_package(cute)
# find_package(physac)
find_package(raylib)
find_package(curl)

add_library(rlimgui STATIC IMPORTED)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(rlimgui PROPERTIES IMPORTED_LOCATION ${rlimgui_LIB_DIR}/rlImGui.lib)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set_target_properties(rlimgui PROPERTIES IMPORTED_LOCATION ${rlimgui_LIB_DIR}/librlImGui.a)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)

ENDIF()

add_library(collishi STATIC IMPORTED)
add_library(cute_headers STATIC IMPORTED)
add_library(physac STATIC IMPORTED)

project(gamethinger)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include_directories(${rlimgui_INCLUDE_DIR})
# link_directories(${rlimgui_LIB_DIR})
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_compile_options(${PROJECT_NAME} PUBLIC "/MP")
    # target_link_options(${PROJECT_NAME} PUBLIC "/FORCE")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_compile_options(${PROJECT_NAME} PUBLIC "-fpermissive")
    # target_link_options(${PROJECT_NAME} PUBLIC "/FORCE")

    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${GTKMM_CFLAGS_OTHER})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
    #target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
ENDIF()
#
target_include_directories(${PROJECT_NAME} PUBLIC ${rapidjson_SOURCE_DIR}/include ${rlimgui_INCLUDE_DIR} ${rlimgui_INCLUDE_DIR}/imgui-master ${collishi_INCLUDE_DIR} ${cute_headers_INCLUDE_DIR} ${physac_INCLUDE_DIR})
# target_link_libraries(${PROJECT_NAME} PRIVATE RapidJSON)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib yaml-cpp enet::enet_static nfd EnTT::EnTT nlohmann_json::nlohmann_json box2d rlimgui CURL::libcurl_static)# ${rlimgui_LIBRARY})
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm.lib urlmon.lib comctl32.lib)
ENDIF()

add_dependencies(${PROJECT_NAME} rlimguiDownload collishiDownload cute_headersDownload physacDownload) #used so rlimgui will be built before linking
