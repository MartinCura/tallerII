//
// Created by milena on 08/11/16.
//


#include <gtest/gtest.h>
#include <PersonManager.h>

#define NAME_DB "/tmp/testDB"

/// Save a new user to DB
TEST(SearchEngine, SearchByName) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);
    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager->savePerson(user, 0);

    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("carlos");
    std::vector<string>* search2 = new std::vector<string>();
    search2->push_back("rodriguez");
    vector<Person*>* user_result = personManager->searchByName(search1);
    vector<Person*>* user_result2 = personManager->searchByName(search2);

    EXPECT_EQ((*user_result)[0]->getFirstName(),"carlos");
    EXPECT_EQ((*user_result2)[0]->getFirstName(), "carlos");

    delete user_result;
    delete user_result2;
    delete search1;
    delete search2;
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}


Person* getFakePerson2() {
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

Person* getFakePerson3() {
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

    Skill* skill2 = new Skill();
    skill2->setName("QA");
    skill2->setDescription("Software Quality Assurance");
    skill2->setCategory("software");
    person->addSkill(skill2);

    return person;
}
TEST(SearchEngine, SearchBySkill) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);
    personManager->savePerson(getFakePerson2()->serializeMe(), 2);
    personManager->savePerson(getFakePerson3()->serializeMe(), 3);

    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("qa");
    std::vector<string>* search2 = new std::vector<string>();
    search2->push_back("javascript");
    vector<Person*>* result = personManager->searchBySkill(search1);
    vector<Person*>* result2 = personManager->searchBySkill(search2);

    EXPECT_EQ((*result).size(), 2);
    EXPECT_EQ((*result2).size(), 1);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete search1;
    delete(search2);
    delete result;
    delete result2;
    delete namedb;

}