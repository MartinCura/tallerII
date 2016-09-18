#ifndef PROJECT_PERSONMANAGER_H
#define PROJECT_PERSONMANAGER_H

#include "Person.h"

class PersonManager {

public:
    PersonManager();
    virtual ~PersonManager();

    Person* getPersonById(int id);

private:
    Person* getFakePerson1();
};

#endif //PROJECT_PERSONMANAGER_H
