#ifndef PROJECT_PERSONMANAGER_H
#define PROJECT_PERSONMANAGER_H

#include "Person.h"
#include "../api/exception/UserNotFoundException.h"

class PersonManager {

public:
    PersonManager();
    virtual ~PersonManager();

    Person* getPersonById(int id);
    void savePerson(Person* person);

private:
    Person* getFakePerson1();
    Person* getFakePerson2();
};

#endif //PROJECT_PERSONMANAGER_H
