############################Test########################################
enable_testing()

set(TARGET ApplicationServer_test)
set(LCOV_PATH "${PROJECT_SOURCE_DIR}/ApplicationServer/src/lcov/bin/lcov")
set(GENHTML_PATH "${PROJECT_SOURCE_DIR}/ApplicationServer/src/lcov/bin/genhtml")
set(PATHS_TO_IGNORE_COVERAGE
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/googletest/googletest/include/gtest/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/googletest/googletest/include/gtest/internal/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/googletest/googletest/src/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/jsoncpp/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/jsoncpp/json/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/db/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/helpers/memenv/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/include/leveldb/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/port/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/table/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/util/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/tests/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/tests/person/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/tests/person/relation/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/tests/person/messages/*"
    "${PROJECT_SOURCE_DIR}/ApplicationServer/src/tests/session/*"
)

message("++ Building tests ++")
message (STATUS "Building application ${TARGET}")

include(CodeCoverage.cmake)

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

add_executable (${TARGET} ${SOURCES}
    testing_uniqueDB.cpp
    ../DB/DBWrapper.h
    ../DB/DBWrapper.cpp
    ../Exceptions/UserAlreadyExistsException.h
    ../Exceptions/UserNotFoundException.h
    ../Exceptions/InvalidTokenException.h
    ../session/SessionManager.cpp
    ../session/SessionManager.h
    ../person/relation/RelationsManager.cpp
    ../person/relation/RelationsManager.h
    ../Exceptions/TokenExpiredException.h
    ../session/Session.cpp
    ../session/Session.h
    ../Security/Security.cpp
    ../Security/Security.h
    ../Exceptions/NotAuthorizedException.h
    ../Mongoose/mongoose.h
    ../logger/Logger.cpp
    ../logger/Logger.h
    ../config/Config.cpp
    ../config/Config.h
    ../api/Response.cpp
    ../api/Response.h
    ../api/WebHandler.cpp
    ../api/WebHandler.h
    ../tools/DbBuilder.cpp
    ../tools/DbBuilder.h
    ../api/handler/AllUsersHandler.cpp
    ../api/handler/ContactsHandler.cpp
    ../api/handler/ConversationsHandler.cpp
    ../api/handler/LoginHandler.cpp
    ../api/handler/LoginWithFacebookHandler.cpp
    ../api/handler/RecommendationsHandler.cpp
    ../api/handler/AllUsersHandler.h
    ../api/handler/ContactsHandler.h
    ../api/handler/Handler.h
    ../api/handler/LoginWithFacebookHandler.h
    ../api/handler/PictureHandler.h
    ../api/handler/RecommendationsHandler.h
    ../api/handler/ConversationsHandler.h
    ../api/handler/NotificationTokenHandler.h
    ../api/handler/LoginHandler.h
    ../api/handler/UsersHandler.h
    ../api/handler/MessagesHandler.h
    ../api/handler/PictureHandler.cpp
    ../api/handler/UsersHandler.cpp
    ../api/handler/Handler.cpp
    ../api/handler/MessagesHandler.cpp
    ../api/handler/NotificationTokenHandler.cpp
)

target_link_libraries (${TARGET} gtest)
target_link_libraries (${TARGET} leveldb)
target_link_libraries (${TARGET} JsonCpp)
target_link_libraries (${TARGET} Mongoose)

add_test (all ${EXECUTABLE_OUTPUT_PATH}/${TARGET})

SETUP_TARGET_FOR_COVERAGE(
        coverage_target     # Name for custom target.
        ${TARGET}           # Name of the test driver executable that runs the tests.
        coverage            # Name of output directory.
)
