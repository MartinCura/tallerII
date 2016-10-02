//
// Created by milena on 28/09/16.
//

#ifndef APPLICATIONSERVER_DBWRAPPER_H
#define APPLICATIONSERVER_DBWRAPPER_H


#include <leveldb/db.h>

class DBWrapper {

public:

    enum ResponseCode {OK, ERROR};

    DBWrapper() { db = nullptr;};
    ~DBWrapper();

    DBWrapper::ResponseCode openDb(std::string nameDB);
    DBWrapper::ResponseCode getKey(std::string key, std::string *output);
    DBWrapper::ResponseCode puTKey(std::string key, std::string *output);
    DBWrapper::ResponseCode deleteKey(std::string key);
    DBWrapper::ResponseCode deleteDB();
    DBWrapper::ResponseCode existsKey(std::string key, std::string *output);
    DBWrapper::ResponseCode destroyDB(std::string nameDB);


private:
    leveldb::DB* db;


};


#endif //APPLICATIONSERVER_DBWRAPPER_HR
