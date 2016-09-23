//
// Created by milena on 23/09/16.
//

#include <gtest/gtest.h>
#include <leveldb/db.h>
#include "../DB/ApplicationSDB.h"

TEST(ApplicationSDB, getLastID) {
    leveldb::DB* db = ApplicationSDB::getInstance();
    std::string result;
    db->Get(leveldb::ReadOptions(), "lastID", &result);

    EXPECT_EQ(std::stoi(result),0);

    delete db;
}

TEST(ApplicationSDB, startDB) {
    leveldb::DB* db = ApplicationSDB::getInstance();
    EXPECT_EQ(db != nullptr, true);
}
