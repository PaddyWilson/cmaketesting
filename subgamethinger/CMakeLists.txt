cmake_minimum_required(VERSION 3.11)

project(subthinger)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# include files from ProjectB
include_directories(${CMAKE_SOURCE_DIR}/gamethinger)
target_link_libraries(${PROJECT_NAME} gamethingerlib)

#install(TARGETS Tutorial1
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib)

#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "../build")
