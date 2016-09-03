cmake_minimum_required(VERSION 3.5.1)
project(ApplicationServer)

include_directories(mongoose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp mongoose/mongoose.h mongoose/mongoose.c api/WebHandler.cpp api/WebHandler.h api/Response.cpp api/Response.h api/UsersHandler.cpp api/UsersHandler.h api/Handler.cpp api/Handler.h)
add_executable(AppServer ${SOURCE_FILES})

install(TARGETS AppServer DESTINATION appserver)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Doria")
include(CPack)
