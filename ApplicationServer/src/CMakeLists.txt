 cmake_minimum_required(VERSION 2.8)

 message (STATUS "Building source code")



set (CMAKE_CXX_FLAGS " -std=c++0x")
set (TARGET ApplicationServer_src)

include_directories ("${PROJECT_SOURCE_DIR}/Mongoose")
include_directories ("${PROJECT_SOURCE_DIR}/jsoncpp")
add_subdirectory (Mongoose)
add_subdirectory (jsoncpp)

file(GLOB_RECURSE SOURCE_FILES main.cpp api/*.cpp api/*.h)

add_executable(${TARGET} ${SOURCE_FILES})
target_link_libraries (${TARGET} Mongoose)
target_link_libraries (${TARGET} jsoncpp)


