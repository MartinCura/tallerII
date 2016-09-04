message (STATUS "Building source code")



set (CMAKE_CXX_FLAGS " -std=c++0x")
set (TARGET ApplicationServer_src)

include_directories ("${PROJECT_SOURCE_DIR}/Mongoose")
add_subdirectory (Mongoose) 

file(GLOB_RECURSE SOURCE_FILES main.cpp api/*.cpp api/*.h)

add_executable(${TARGET} ${SOURCE_FILES})
target_link_libraries (${TARGET} Mongoose)


