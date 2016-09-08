cmake_minimum_required(VERSION 2.8)
project(json_library)

#Bring the headers, such as Student.h into the project
include_directories(json)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "*.cpp")

#Generate the static library from the sources
add_library(JsonCpp STATIC ${SOURCES})

#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply	
install (TARGETS JsonCpp DESTINATION "${CMAKE_BINARY_DIR}/lib")