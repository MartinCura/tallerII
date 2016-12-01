# Set the minimum version needed
cmake_minimum_required (VERSION 2.8)

# Set the name of the project
project (ApplicationServer)

message ("** ApplicationServer **")



# set this variable to specify a common place where CMake should put all executable files
# (instead of CMAKE_CURRENT_BINARY_DIR)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# if you are building in-source, this is the same as CMAKE_CURRENT_SOURCE_DIR, otherwise this 
# is the directory where the compiled or generated files from the current CMakeLists.txt will go to. In this case, as we are going to call the cmake #command from the build directory -> ${CMAKE_CURRENT_BINARY_DIR} == build/


message ( STATUS "BUILD DIRECTORY:		" ${CMAKE_CURRENT_BINARY_DIR})
message ( STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH} )
message ( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )
MESSAGE( STATUS " " ${} )

enable_testing()

add_subdirectory(ApplicationServer)