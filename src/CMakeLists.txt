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
find_package(json)
# find_package(cute)
# find_package(physac)
find_package(raylib)
find_package(curl)

add_library(rlimgui STATIC IMPORTED)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(rlimgui PROPERTIES IMPORTED_LOCATION ${rlimgui_LIB_DIR}/rlImGui.lib)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set_target_properties(rlimgui PROPERTIES IMPORTED_LOCATION ${rlimgui_LIB_DIR}/librlImGui.a)
ENDIF()

add_library(collishi STATIC IMPORTED)
add_library(cute_headers STATIC IMPORTED)
add_library(physac STATIC IMPORTED)

project(Tutorial1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include_directories(${rlimgui_INCLUDE_DIR})
# link_directories(${rlimgui_LIB_DIR})
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} "enetTest.cpp")

if(CMAKE_SYSTEM_NAME MATCHES "Windoes")
    target_link_libraries(${PROJECT_NAME} winmm.lib)    
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC "${enet_SOURCE_DIR}/include" "${rapidjson_SOURCE_DIR}/include" ${rlimgui_INCLUDE_DIR} "${rlimgui_INCLUDE_DIR}/imgui-master" ${collishi_INCLUDE_DIR} ${cute_headers_INCLUDE_DIR} ${physac_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} raylib yaml-cpp nfd EnTT::EnTT nlohmann_json::nlohmann_json box2d rlimgui CURL::libcurl)# ${rlimgui_LIBRARY})
add_dependencies(${PROJECT_NAME} rlimguiDownload collishiDownload cute_headersDownload physacDownload)#used so rlimgui will be built before linking