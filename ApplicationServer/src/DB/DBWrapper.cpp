//
// Created by milena on 28/09/16.
//

#include <iostream>
#include "DBWrapper.h"

DBWrapper::ResponseCode DBWrapper::openDb(std::string nameDB) {
    if (db != nullptr) {
        throw std::exception(); //No se debería abrir una base que ya está abierta.
    }

    leveldb::Options options;
    leveldb::Status s;

    options.create_if_missing = true;
    s = leveldb::DB::Open(options, nameDB, &db);
    if (!s.ok()) {
        return ERROR;
    }
    return OK;
}


DBWrapper::ResponseCode DBWrapper::getKey(std::string key, std::string *output) {
    if (!db) {
        //Primero se tiene que iniciar la base
        throw std::exception();
    }

    leveldb::Status s = db->Get(leveldb::ReadOptions(), key, output);

    if (!s.ok()) {
        return ERROR;
    }

    return OK;
}

DBWrapper::ResponseCode DBWrapper::puTKey(std::string key, std::string *output) {
    if (!db) {
        throw std::exception(); //Se debe primero iniciar la base
    }

    leveldb::Status s;

    s = db->Put(leveldb::WriteOptions(), key, *output );

    if (!s.ok()) {
        return ERROR;
    }

    return OK;
}

DBWrapper::ResponseCode DBWrapper::deleteKey(std::string key) {
    if (!db) {
        throw std::exception(); //Se debe primero iniciar la base
    }

    leveldb::Status s;
    s = db->Delete(leveldb::WriteOptions(), key);

    if (!s.ok()) {
        return ERROR;
    }

    return OK;
}

DBWrapper::ResponseCode DBWrapper::deleteDB() {
    if (!db) {
        return OK;
    }

    delete db;
    db = nullptr;
}

DBWrapper::~DBWrapper() {
    if (db != nullptr) {
        deleteDB();
    }
}

DBWrapper::ResponseCode DBWrapper::existsKey(std::string key, std::string *output) {
    bool s;
    s = db->Get(leveldb::ReadOptions(), key, output).IsNotFound();
    if (!s) {
        return ERROR;
    }

    return OK;
}

DBWrapper::ResponseCode DBWrapper::destroyDB(std::string nameDB) {
    leveldb::DestroyDB(nameDB, leveldb::Options());
    return OK;
}



