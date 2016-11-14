message(STATUS "Building tests")

set(TARGET ApplicationServer_test)
set(LCOV_PATH "${PROJECT_SOURCE_DIR}/ApplicationServer/src/lcov/bin/lcov")
set(GENHTML_PATH "${PROJECT_SOURCE_DIR}/ApplicationServer/src/lcov/bin/genhtml")
message (STATUS "Building application ${TARGET}")

include(${PROJECT_SOURCE_DIR}/ApplicationServer/src/tests/CodeCoverage.cmake)

include_directories ("${PROJECT_SOURCE_DIR}/leveldb/db")
include_directories ("${PROJECT_SOURCE_DIR}/leveldb/include/")
include_directories ("${PROJECT_SOURCE_DIR}/leveldb/leveldb/util")
include_directories ("${PROJECT_SOURCE_DIR}/leveldb/leveldb/port")
include_directories ("${PROJECT_SOURCE_DIR}/lcov")

set (SOURCES testing_levedb.cpp)

add_executable (${TARGET} ${SOURCES})
target_link_libraries (${TARGET} leveldb)
add_test (all ${EXECUTABLE_OUTPUT_PATH}/${TARGET})


SETUP_TARGET_FOR_COVERAGE(
        coverage_all        # Name for custom target.
        ${TARGET}           # Name of the test driver executable that runs the tests.
        coverage            # Name of output directory.
)

