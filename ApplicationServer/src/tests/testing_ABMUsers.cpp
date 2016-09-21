#include <string>
#include "gtest/gtest.h"
#include "leveldb/db.h"
#include <iostream>

TEST(KeyFound, KeyNotStored) {

///Error when asking fot user that doesn't exist
	leveldb::DB* db;
	leveldb::Options op;
	op.create_if_missing = true;
	leveldb::Status s = leveldb::DB::Open(op, "/tmp/testingleveldb", &db);
	EXPECT_EQ(s.ok(), true);
	std::string value;
	leveldb::Status sGet = db->Get(leveldb::ReadOptions(), "key_not_saved", &value);
	EXPECT_EQ(sGet.IsNotFound(), true);

	delete db;

}

/// Save New User that doesn't exist already
TEST(SaveUser, NotExists) {
	EXPECT_EQ(1,1);

}


/// Save New User that already exists
TEST(SaveUser, Exists) {
	EXPECT_EQ(1,1);

}


/// Get  User that doesn't exist already
TEST(GetUser, NotExists) {
	EXPECT_EQ(1,1);

}

/// Get  User that exists already
TEST(GetUser, Exists) {
	EXPECT_EQ(1,1);

}

/// Delete User that doesn't exist already
TEST(DeleteUser, NotExists) {
    EXPECT_EQ(1,1);

}

/// Delete User that exists already
TEST(DeleteUser, Exists) {
    EXPECT_EQ(1,1);

}
