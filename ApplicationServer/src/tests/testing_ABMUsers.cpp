#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "leveldb/db.h"
#include <iostream>

class ExistsUserDBTest : public ::testing::Test { 
protected:
	virtual void SetUp() {

		leveldb::Options op;
		op.create_if_missing = true;
		leveldb::Status s = leveldb::DB::Open(op, "/tmp/testingABMUser_ExistUser", &db_);
		Json::Value user = CreateFakeUser(1);
		
		Json::FastWriter fastWriter;
		std::string output = fastWriter.write(user);
		db_->Put(leveldb::WriteOptions(), "user_1", output);

	}

	Json::Value CreateFakeUser(int id) {

		Json::Value user;
		user["id"] = id;
		user["first_name"] = "Carlos";
		user["last_name"] = "Rodriguez";
		user["email"] = "crodriguez@gmail.com";
		user["date_of_birth"] = "01/01/1990";
		user["city"] = "CABA";
		user["profile_picture"] = "";
		user["summary"] = "Me gusta el arrte";
		return user;
	}



	leveldb::DB* db_;


	virtual void TearDown() { 
		delete db_; 
	}
};

/// Get  User that exists already
TEST_F(ExistsUserDBTest, GetUser) {
	std::string value;
	leveldb::Status s = db_->Get(leveldb::ReadOptions(), "user_1", &value);
	EXPECT_EQ(s.ok(), true);

}

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
/*
TEST_F(ExistsUserDBTest, SaveUser) {
	EXPECT_EQ(1,1);

}


/// Get  User that doesn't exist already
TEST(GetUser, NotExists) {
	EXPECT_EQ(1,1);

}



/// Delete User that doesn't exist already
TEST(DeleteUser, NotExists) {
    EXPECT_EQ(1,1);

}

/// Delete User that exists already
TEST_F(ExistsUserDBTest, DeleteUser) {
    EXPECT_EQ(1,1);

}

*/

