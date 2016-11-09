//
// Clase creada con el objetivo de evitar la dependecia directa con LevelDB.
//

#include <iostream>
#include "DBWrapper.h"

DBWrapper* DBWrapper::db1 = NULL;

DBWrapper * DBWrapper::openDb(std::string* nameDB) {
    if (DBWrapper::db1 == nullptr) {
        db1 = new DBWrapper(nameDB); //No se debería abrir una base que ya está abierta.
    }
    return db1;
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


DBWrapper::ResponseCode DBWrapper::puTKey(std::string key, std::string *value) {
    if (!db) {
        throw std::exception(); //Se debe primero iniciar la base
    }

    leveldb::Status s;

    s = db->Put(leveldb::WriteOptions(), key, *value );

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

DBWrapper::ResponseCode DBWrapper::existsKey(std::string key, std::string *output) {
    bool s;
    s = db->Get(leveldb::ReadOptions(), key, output).IsNotFound();
    if (!s) {
        return ERROR;
    }

    return OK;
}


leveldb::Iterator* DBWrapper::newIterator() {
    return db->NewIterator(leveldb::ReadOptions());

}

void DBWrapper::ResetInstance() {
    delete db1;
    db1 = nullptr;

}

DBWrapper::DBWrapper(std::string* nameDB) {
    leveldb::Options options;
    options.create_if_missing = true;
    //options.error_if_exists = true;
    leveldb::Status s = leveldb::DB::Open(options, *nameDB, &db);
    //std::cout << s.ToString();
    std::string mensaje = s.ToString();
    //std::cout << mensaje;
}

DBWrapper::ResponseCode DBWrapper::destroyDB(std::string *nameDB) {
    leveldb::DestroyDB(*nameDB +"/", leveldb::Options());
}


