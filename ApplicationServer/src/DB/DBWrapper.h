//
// Created by milena on 28/09/16.
//

#ifndef APPLICATIONSERVER_DBWRAPPER_H
#define APPLICATIONSERVER_DBWRAPPER_H


#include <leveldb/db.h>

class DBWrapper {

public:

    enum ResponseCode {OK, ERROR};


    static void ResetInstance();

    static DBWrapper * openDb(std::string *nameDB);

    DBWrapper::ResponseCode getKey(std::string key, std::string *output);
    DBWrapper::ResponseCode puTKey(std::string key, std::string *output);
    DBWrapper::ResponseCode deleteKey(std::string key);
    DBWrapper::ResponseCode existsKey(std::string key, std::string *output);
    DBWrapper::ResponseCode destroyDB(std::string *nameDB);
    leveldb::Iterator * newIterator();
    static DBWrapper* db1;
    leveldb::DB* db = nullptr;

private:
    DBWrapper(std::string* nameDB);
    ~DBWrapper() {
        delete db;
        db = nullptr;
    };




};


#endif //APPLICATIONSERVER_DBWRAPPER_HR
