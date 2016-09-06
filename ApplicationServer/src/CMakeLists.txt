message (STATUS "Building source code")

set (CMAKE_CXX_FLAGS " -std=c++0x")
set (TARGET ApplicationServer_src)

# Used to bring the header files into the build environment
include_directories ("${PROJECT_SOURCE_DIR}/jsoncpp")
include_directories ("${PROJECT_SOURCE_DIR}/Mongoose")
link_directories("${CMAKE_BINARY_DIR}/lib")


add_subdirectory(Mongoose)
add_subdirectory(jsoncpp)
add_subdirectory(leveldb)

# Used to set a variable(SOURCE_FILES) that contains the name values of all of the source
# files (.cpp) in the project
file(GLOB_RECURSE SOURCE_FILES main.cpp api/*.cpp)

# Requests that an executable is to be built using ${SOURCE_FILES}. 
# The first argument is the name of the executable
# The second argument are the source files from wich to build the executable
add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries (${TARGET} Mongoose)
target_link_libraries (${TARGET} Peterete)

