cmake_minimum_required(VERSION 2.8)
PROJECT(leveldb)


set(VERSION_MAJOR 1)
set(VERSION_MINOR 15)
set(VERRION_PATCH 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR})

# ${CMAKE_CURRENT_SOURCE_DIR}  -> This is the directoy where the urrently processed CMakeLists.txt ie located in.
include_directories(${CMAKE_CURRENT_SOURCE_DIR} include)

message("Configuration: ${CMAKE_BUILD_TYPE}")
## needs define:
## 	COMPILER_MSVC
## 	LEVELDB_PLATFORM_POSIX
## 	LEVELDB_CSTDATOMIC_PRESENT
## 	SNAPPY
## 	OS_****

add_definitions(-DOS_LINUX)
add_definitions(-DLEVELDB_PLATFORM_POSIX)

add_definitions(-fno-builtin-memcmp)

set(PORT_SRCS port/port_posix.cc util/env_posix.cc)
set(PORT_HDRS port/port_posix.h util/posix_logger.h)

set(CAPI_SRC db/c.cc)
set(CAPI_HDRS include/leveldb/c.h)
set(CAPI_TEST db/c_test.c)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")



set(SRCS
  db/builder.cc
  db/db_impl.cc
  db/db_iter.cc
  db/dbformat.cc
  db/filename.cc
  db/log_reader.cc
  db/log_writer.cc
  db/memtable.cc
  db/repair.cc
  db/table_cache.cc
  db/version_edit.cc
  db/version_set.cc
  db/write_batch.cc
  helpers/memenv/memenv.cc
  table/block_builder.cc
  table/block.cc
  table/filter_block.cc
  table/format.cc
  table/iterator.cc
  table/merger.cc
  table/table_builder.cc
  table/table.cc
  table/two_level_iterator.cc
  util/arena.cc
  util/bloom.cc
  util/cache.cc
  util/coding.cc
  util/comparator.cc
  util/crc32c.cc
  util/env.cc
  util/filter_policy.cc
  util/hash.cc
  util/histogram.cc
  util/logging.cc
  util/options.cc
  util/status.cc
  ${PORT_SRCS}
  ${CAPI_SRC}
  )

set(HDRS
  db/builder.h
  db/db_impl.h
  db/db_iter.h
  db/dbformat.h
  db/filename.h
  db/log_format.h
  db/log_reader.h
  db/log_writer.h
  db/memtable.h
  db/skiplist.h
  db/snapshot.h
  db/table_cache.h
  db/version_edit.h
  db/version_set.h
  db/write_batch_internal.h
  helpers/memenv/memenv.h
  include/leveldb/cache.h
  include/leveldb/comparator.h
  include/leveldb/db.h
  include/leveldb/env.h
  include/leveldb/filter_policy.h
  include/leveldb/iterator.h
  include/leveldb/options.h
  include/leveldb/slice.h
  include/leveldb/status.h
  include/leveldb/table.h
  include/leveldb/table_builder.h
  include/leveldb/write_batch.h
  port/atomic_pointer.h
  port/port.h
  port/thread_annotations.h
  table/block.h
  table/block_builder.h
  table/filter_block.h
  table/format.h
  table/iterator_wrapper.h
  table/merger.h
  table/two_level_iterator.h
  util/arena.h
  util/coding.h
  util/crc32c.h
  util/hash.h
  util/histogram.h
  util/logging.h
  util/mutexlock.h
  util/random.h
  ${PORT_HDRS}
  ${CAPI_HDRS}
  )

add_library(leveldb SHARED ${SRCS} ${HDRS} ${SNAPPY_SRCS} ${SNAPPY_HDRS})
set_target_properties(leveldb PROPERTIES
  VERSION ${VERSION_STRING}
  SOVERSION ${VERSION_MAJOR}
  )
install (TARGETS leveldb DESTINATION "${CMAKE_BINARY_DIR}/lib")
find_package(Threads)
target_link_libraries(leveldb ${CMAKE_THREAD_LIBS_INIT} ${LIBS})
