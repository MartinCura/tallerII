############################Test########################################
enable_testing()

set(TARGET ApplicationServer_test)

message("++ Building tests ++")
message (STATUS "Building application ${TARGET}")

include_directories ("${gtest_SOURCE_DIR}/include")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/jsoncpp")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/person")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/db")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/include")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/leveldb/util")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/leveldb/port")

# Used to set a variable(SOURCE_FILES) that contains the name values of all of the source
# files (.cpp) in the project
file(GLOB_RECURSE SOURCES *.cpp ../person/*.cpp)

add_executable (${TARGET} ${SOURCES} ../DB/DBWrapper.h ../DB/DBWrapper.cpp testing_uniqueDB.cpp ../Exceptions/UserAlreadyExistsException.h ../Exceptions/UserNotFoundException.h ../Exceptions/InvalidTokenException.h initdb.cpp ../session/SessionManager.cpp ../session/SessionManager.h  ../person/relation/RelationsManager.cpp ../person/relation/RelationsManager.h ../Exceptions/TokenExpiredException.h ../session/Session.cpp ../session/Session.h ../Security/Security.cpp ../Security/Security.h ../Exceptions/NotAuthorizedException.h)

target_link_libraries (${TARGET} gtest)
target_link_libraries (${TARGET} leveldb)
target_link_libraries (${TARGET} JsonCpp)

add_test (all ${EXECUTABLE_OUTPUT_PATH}/${TARGET})
