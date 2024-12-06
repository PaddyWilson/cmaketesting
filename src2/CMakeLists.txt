cmake_minimum_required(VERSION 3.11)

project(Tutorial2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_library(Tutorial2lib STATIC ${SOURCE_FILES})
#add_library(Tutorial2lib.b SHARED ${SOURCE_FILES})

#include_directories(${CMAKE_SOURCE_DIR}/projectB) # include files from ProjectB
#target_link_libraries(projectA projectB)

install(TARGETS Tutorial2lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)