#include "PersonManager.h"

PersonManager::PersonManager() {}

PersonManager::~PersonManager() {}

Person* PersonManager::getPersonById(int id) {
    //FIXME: sacar una vez que este la base de datos
    switch (id) {
        case 1:
            return this->getFakePerson1();
        case 2:
            return this->getFakePerson2();
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

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("IBM");
    workHistory1->setPositionTitle("JavaScript Developer");
    workHistory1->setFromDate("10/2012");
    workHistory1->setToDate("11/2014");
    person->addWorkHistory(workHistory1);

    WorkHistory* workHistory2 = new WorkHistory();
    workHistory2->setCompany("Amazon");
    workHistory2->setPositionTitle("Project Manager");
    workHistory2->setFromDate("12/2014");
    workHistory2->setToDate("");
    person->addWorkHistory(workHistory2);

    Skill* skill1 = new Skill();
    skill1->setName("JavaScript");
    skill1->setDescription("JavaScrip programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("PM");
    skill2->setDescription("Project Management");
    skill2->setCategory("management");
    person->addSkill(skill2);

    return person;
}

//FIXME: sacar una vez que este la base de datos
Person* PersonManager::getFakePerson2() {
    Person* person = new Person();
    person->setFirstName("Jane");
    person->setLastName("Doe");
    person->setEmail("Jane@doe.com");
    person->setDateOfBirth("01/11/1991");
    person->setCity("Madrid");
    person->setProfilePicture("");
    person->setSummary("Hi, this is Jane Doe");

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("Microsoft");
    workHistory1->setPositionTitle("QA");
    workHistory1->setFromDate("10/2013");
    workHistory1->setToDate("11/2015");
    person->addWorkHistory(workHistory1);

    WorkHistory* workHistory2 = new WorkHistory();
    workHistory2->setCompany("E-bay");
    workHistory2->setPositionTitle("Php Developer");
    workHistory2->setFromDate("12/2015");
    workHistory2->setToDate("");
    person->addWorkHistory(workHistory2);

    Skill* skill1 = new Skill();
    skill1->setName("Php");
    skill1->setDescription("Php programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("QA");
    skill2->setDescription("Software Quality Assurance");
    skill2->setCategory("software");
    person->addSkill(skill2);

    return person;
}
