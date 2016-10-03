message (STATUS "Building source code")

set (CMAKE_CXX_FLAGS " -std=c++0x")
set (TARGET ApplicationServer_src)

# Used to bring the header files into the build environment
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/jsoncpp")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/Mongoose")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/db")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/include")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/leveldb/util")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/leveldb/port")

link_directories("${CMAKE_BINARY_DIR}/lib")


add_subdirectory(Mongoose)
add_subdirectory(jsoncpp)
add_subdirectory(leveldb)
add_subdirectory(googletest)
add_subdirectory(tests)

# Used to set a variable(SOURCE_FILES) that contains the name values of all of the source
# files (.cpp) in the project
file(GLOB_RECURSE SOURCE_FILES main.cpp api/*.cpp logger/*.cpp person/*.cpp api/handler/*.cpp )

# Requests that an executable is to be built using ${SOURCE_FILES}. 
# The first argument is the name of the executable
# The second argument are the source files from wich to build the executable
add_executable(${TARGET} ${SOURCE_FILES} DB/ApplicationSDB.cpp DB/ApplicationSDB.h Exceptions/UserAlreadyExistsException.h  Exceptions/UserNotFoundException.h DB/DBWrapper.cpp DB/DBWrapper.h api/handler/LoginHandler.cpp api/handler/LoginHandler.h Exceptions/InvalidPasswordException.h session/SessionManager.cpp session/SessionManager.h)

target_link_libraries (${TARGET} Mongoose)
target_link_libraries (${TARGET} JsonCpp)
target_link_libraries (${TARGET} leveldb)

# Creates a directory for log files
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/ApplicationServer/logs")
execute_process(COMMAND /bin/chmod 777 "${PROJECT_SOURCE_DIR}/ApplicationServer/logs")
