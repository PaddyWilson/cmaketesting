cmake_minimum_required(VERSION 3.11)

project(Tutorial3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# include files from ProjectB
include_directories(${CMAKE_SOURCE_DIR}/src2)
target_link_libraries(Tutorial3 Tutorial2lib)

#install(TARGETS Tutorial1
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib)

#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "../build")
