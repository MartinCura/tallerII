############################Test########################################
enable_testing()

set(TARGET ApplicationServer_test)

message("++ Building tests ++")
message (STATUS "Building application ${TARGET}")

include_directories ("${gtest_SOURCE_DIR}/include")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/db")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/include")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/leveldb/util")
include_directories ("${PROJECT_SOURCE_DIR}/ApplicationServer/src/leveldb/leveldb/port")

set (SOURCES gtest_main.cpp testing_gtest.cpp testing_leveldb.cpp)

add_executable (${TARGET} ${SOURCES} )

target_link_libraries (${TARGET} gtest)
target_link_libraries (${TARGET} leveldb)

add_test (all ${EXECUTABLE_OUTPUT_PATH}/${TARGET})
