#ifndef PROJECT_DBBUILDER_H
#define PROJECT_DBBUILDER_H

#include "../person/Person.h"
#include "../person/PersonManager.h"
#include "../logger/Logger.h"
#include "../Exceptions/UserAlreadyExistsException.h"

using namespace std;

class DbBuilder {

public:
    DbBuilder();
    ~DbBuilder();

    void loadUsers();
    Person* getFakePerson1();
    Person* getFakePerson2();
    Person* getFakePerson3();

private:
    DBWrapper* db;
    string *namedb;
    void saveToken(string token, string user_mail);
};

#endif //PROJECT_DBBUILDER_H
