//
// Created by milena on 23/09/16.
//

#include "ApplicationSDB.h"


leveldb::DB *ApplicationSDB::uniqueInstance = nullptr;

ApplicationSDB::~ApplicationSDB() {}

leveldb::DB *ApplicationSDB::getInstance() {
    if (uniqueInstance == nullptr) {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &uniqueInstance);

        status = uniqueInstance->Put(leveldb::WriteOptions(), "lastID", std::to_string(0));
    }

    return uniqueInstance;
}
