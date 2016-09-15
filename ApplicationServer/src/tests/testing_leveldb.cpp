#include <string>
#include "gtest/gtest.h"
#include "leveldb/db.h"
#include <iostream>

TEST(LevelDB, WorkinGLevelDB) {
 
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
	EXPECT_EQ(status.ok(), true);

	std::string str1("green");

	status = db->Put(leveldb::WriteOptions(), "Key", str1);
	std::string result;

	db->Get(leveldb::ReadOptions(), "Key", &result);

	EXPECT_EQ(str1.compare(result), 0);
}