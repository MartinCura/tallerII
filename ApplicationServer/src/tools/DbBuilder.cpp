#include "DbBuilder.h"

#define NAME_DB "/tmp/appDB/"

DbBuilder::DbBuilder() {}

DbBuilder::~DbBuilder() {}

void DbBuilder::loadUsers() {
    PersonManager *personManager = new PersonManager(NAME_DB);
    try {
        Person* person1 = this->getFakePerson1();
        personManager->savePerson(0, person1->serializeMe(), (long) 1);
        delete person1;
    } catch (UserAlreadyExistsException &exception) {}
    try {
        Person* person2 = this->getFakePerson2();
        personManager->savePerson(0, person2->serializeMe(), (long) 2);
        delete person2;
    } catch (UserAlreadyExistsException &exception) {}
    try {
        Person* person3 = this->getFakePerson3();
        personManager->savePerson(0, person3->serializeMe(), (long) 3);
        delete person3;
    } catch (UserAlreadyExistsException &exception) {}
    delete personManager;
}

Person* DbBuilder::getFakePerson1() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("John");
    person->setLastName("Doe");
    person->setEmail("john@doe.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/01/1990");
    person->setCity("Buenos Aires");
    person->setSummary("Hi, this is John Doe");
    person->setLocation(-58.368368, -34.617589);

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
    skill1->setDescription("JavaScript programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("PM");
    skill2->setDescription("Project Management");
    skill2->setCategory("management");
    person->addSkill(skill2);

    return person;
}

Person* DbBuilder::getFakePerson2() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Jane");
    person->setLastName("Doe");
    person->setEmail("jane@doe.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/11/1991");
    person->setCity("Madrid");
    person->setSummary("Hi, this is Jane Doe");
    person->setLocation(-58.368368, -34.617589);

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("Microsoft");
    workHistory1->setPositionTitle("QA");
    workHistory1->setFromDate("10/2013");
    workHistory1->setToDate("11/2015");
    person->addWorkHistory(workHistory1);

    WorkHistory* workHistory2 = new WorkHistory();
    workHistory2->setCompany("E-bay");
    workHistory2->setPositionTitle("PHP Developer");
    workHistory2->setFromDate("12/2015");
    workHistory2->setToDate("");
    person->addWorkHistory(workHistory2);

    Skill* skill1 = new Skill();
    skill1->setName("Php");
    skill1->setDescription("PHP programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("QA");
    skill2->setDescription("Software Quality Assurance");
    skill2->setCategory("software");
    person->addSkill(skill2);

    return person;
}

Person* DbBuilder::getFakePerson3() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Carlos");
    person->setLastName("Rodriguez");
    person->setEmail("crodriguez@gmail.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/01/1990");
    person->setCity("CABA");
    person->setSummary("Hola, soy Carlos Rodriguez");
    person->setLocation(-58.368368, -34.617589);

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("QA");
    workHistory1->setPositionTitle("Microsoft");
    workHistory1->setFromDate("10/2012");
    workHistory1->setToDate("11/2014");
    person->addWorkHistory(workHistory1);

    Skill* skill1 = new Skill();
    skill1->setName("JavaScript");
    skill1->setDescription("JavaScript programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    return person;
}
