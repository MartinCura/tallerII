#ifndef PROJECT_PERSONMANAGER_H
#define PROJECT_PERSONMANAGER_H

#include <leveldb/db.h>
#include "Person.h"
#include "../Exceptions/UserNotFoundException.h"
#include "../DB/DBWrapper.h"
#include "../Exceptions/InvalidRequestException.h"

class PersonManager {

public:
    PersonManager(std::string nameDB);
    virtual ~PersonManager();

    Person* getPersonById(long id);
    Person* getPersonByMail(string* user_mail);
    void destroyDB();

    long savePerson(Json::Value person_json, long forceID = -1);
    vector<long> * getAllUsersIds();
    void deletePerson(long id);

private:
    DBWrapper* db;
    std::string nameDB;
    long uniqueId;

    long generateID();


    vector<Person *> getPersonFriendsById(long id);


};

#endif //PROJECT_PERSONMANAGER_H
