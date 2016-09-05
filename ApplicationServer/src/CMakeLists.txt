message (STATUS "Building source code")

set (CMAKE_CXX_FLAGS " -std=c++0x")
set (TARGET ApplicationServer_src)

include_directories ("${PROJECT_SOURCE_DIR}/Mongoose")
include_directories ("${PROJECT_SOURCE_DIR}/jsoncpp")
add_subdirectory (Mongoose)
add_subdirectory (jsoncpp)

file(GLOB_RECURSE SOURCE_FILES main.cpp api/*.cpp api/*.h)

# Requests that an executable is to be built using ${SOURCE_FILES}. 
# The first argument is the name of the executable
# The second argument are the source files from wich to build the executable
add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries (${TARGET} Mongoose)
target_link_libraries (${TARGET} jsoncpp)


