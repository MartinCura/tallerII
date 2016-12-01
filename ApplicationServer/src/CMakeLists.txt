message (STATUS "Building source code")

set (CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -std=c++0x")
set (TARGET ApplicationServer_src)
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "David Doria")

include(CPack)

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
file(GLOB_RECURSE SOURCE_FILES
    main.cpp
    api/*.cpp
    api/*.h
    logger/*.cpp
    person/*.cpp
    api/handler/*.cpp
    api/handler/*.h
    tools/*.cpp
    config/*.cpp
)

# Requests that an executable is to be built using ${SOURCE_FILES}. 
# The first argument is the name of the executable
# The second argument are the source files from wich to build the executable
add_executable(${TARGET} ${SOURCE_FILES}
    DB/ApplicationSDB.cpp
    DB/ApplicationSDB.h
    Exceptions/UserAlreadyExistsException.h
    Exceptions/UserNotFoundException.h
    DB/DBWrapper.cpp
    DB/DBWrapper.h
    api/handler/LoginHandler.cpp
    api/handler/LoginHandler.h
    Exceptions/InvalidPasswordException.h
    session/SessionManager.cpp
    session/SessionManager.h
    Exceptions/InvalidTokenException.h
    Exceptions/TokenExpiredException.h
    session/Session.cpp
    session/Session.h
    Security/Security.cpp
    Security/Security.h
    Exceptions/NotAuthorizedException.h
    Exceptions/BadImplementationException.h
    api/handler/TopTenHandler.cpp
    api/handler/TopTenHandler.h
    api/handler/SearchHandler.cpp
    api/handler/SearchHandler.h
    person/Manager.cpp
    person/Manager.h
    Exceptions/KeyNotFound.h
)

target_link_libraries (${TARGET} Mongoose)
target_link_libraries (${TARGET} JsonCpp)
target_link_libraries (${TARGET} leveldb)

set (INSTALLATION_DIR appServer)

# Creates a directory for log files

file(MAKE_DIRECTORY "/usr/${INSTALLATION_DIR}/logs")
execute_process(COMMAND /bin/chmod 777 "/usr/${INSTALLATION_DIR}/logs")

execute_process(COMMAND cp -rf "${PROJECT_SOURCE_DIR}/ApplicationServer/img" "/usr/${INSTALLATION_DIR}")
execute_process(COMMAND cp "${PROJECT_SOURCE_DIR}/ApplicationServer/src/config.js" "/usr/${INSTALLATION_DIR}")

install(TARGETS ${TARGET} DESTINATION ${INSTALLATION_DIR})
install(FILES ${PROJECT_SOURCE_DIR}/ApplicationServer/src/config.js DESTINATION ${INSTALLATION_DIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/ApplicationServer/img DESTINATION ${INSTALLATION_DIR})
install(
    DIRECTORY /usr/${INSTALLATION_DIR}/logs
    DIRECTORY_PERMISSIONS
	OWNER_READ
	OWNER_WRITE
	OWNER_EXECUTE
	GROUP_READ
	GROUP_WRITE
	GROUP_EXECUTE
	WORLD_READ
	WORLD_WRITE
	WORLD_EXECUTE
    DESTINATION ${INSTALLATION_DIR}
)
