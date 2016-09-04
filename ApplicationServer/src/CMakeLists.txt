message (STATUS "Building source code")

set(target ApplicationServer_src)
set(sources main.cpp)

add_executable(${target} ${sources})