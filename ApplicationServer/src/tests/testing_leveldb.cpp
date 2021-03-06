#include <string>
#include "gtest/gtest.h"
#include "leveldb/db.h"
#include <iostream>

TEST(LevelDB, WorkinGLevelDB) {
 
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	//options.error_if_exists = true;
	leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb/", &db);

    //std::cout << status.ToString();
    EXPECT_EQ(status.ok(), true);
	std::string str1("green");
    if (status.ok()) {
        status = db->Put(leveldb::WriteOptions(), "Key", str1);
        std::string result;

        db->Get(leveldb::ReadOptions(), "Key", &result);

        EXPECT_EQ(str1.compare(result), 0);
    }

    delete db;
    leveldb::DestroyDB("/tmp/testdb/", leveldb::Options());
}