message (STATUS "Building source code")

include_directories(Mongoose)

set(CMAKE_CXX_FLAGS "-std=c++11")
set (TARGET ApplicationServer_src)

set(SOURCE_FILES main.cpp Mongoose/mongoose.h Mongoose/mongoose.c api/WebHandler.cpp api/WebHandler.h api/Response.cpp api/Response.h api/UsersHandler.cpp api/UsersHandler.h api/Handler.cpp api/Handler.h)

add_executable(${TARGET} ${SOURCE_FILES})


