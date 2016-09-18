#include "PersonManager.h"

PersonManager::PersonManager() {}

PersonManager::~PersonManager() {}

Person* PersonManager::getPersonById(int id) {
    //FIXME: muy feo
    switch (id) {
        case 1:
            return this->getFakePerson1();
    }
}

//FIXME: sacar una vez que este la base de datos
Person* PersonManager::getFakePerson1() {
    Person* person = new Person();
    person->setFirstName("John");
    person->setLastName("Doe");
    person->setEmail("John@doe.com");
    person->setDateOfBirth("01/01/1990");
    person->setCity("Buenos Aires");
    person->setProfilePicture("");
    person->setSummary("Hi, this is John Doe");
    return person;
}

