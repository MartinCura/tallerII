# Set the minimum version needed
cmake_minimum_required (VERSION 2.8)

# Set the name of the project
project (ApplicationServer)

enable_testing()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(ApplicationServer)

