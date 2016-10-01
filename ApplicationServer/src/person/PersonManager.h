#ifndef PROJECT_PERSONMANAGER_H
#define PROJECT_PERSONMANAGER_H

#include <leveldb/db.h>
#include "Person.h"
#include "../Exceptions/UserNotFoundException.h"
#include "../Exceptions/InvalidRequestException.h"

class PersonManager {

public:
    PersonManager();
    virtual ~PersonManager();

    Person* getPersonById(int id);
    Person* getPersonById(long id);
    Person* getPersonByMail(string* user_mail);

    long savePerson(Json::Value person_json);
    void deletePerson(long id);

private:
    leveldb::DB* db;
    long uniqueId;

    Person* getFakePerson1();
    Person* getFakePerson2();

    bool userExists(long id, string *result);
    bool userExists(string *user_mail, string *result);


};

#endif //PROJECT_PERSONMANAGER_H
