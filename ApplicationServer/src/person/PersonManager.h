#ifndef PROJECT_PERSONMANAGER_H
#define PROJECT_PERSONMANAGER_H

#include <leveldb/db.h>
#include "Person.h"
#include "../Exceptions/UserNotFoundException.h"

class PersonManager {

public:
    PersonManager();
    virtual ~PersonManager();

    Person* getPersonById(int id);
    Person *getPersonById(long id);
    void savePerson(Json::Value person_json);

private:
    leveldb::DB* db;
    Person* getFakePerson1();
    Person* getFakePerson2();

    long uniqueId;

    bool userExists(long id, string *result);

    bool userExists(string *user_mail, string *result);

    void deletePerson(long id);
};

#endif //PROJECT_PERSONMANAGER_H
