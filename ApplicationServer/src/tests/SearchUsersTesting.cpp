//
// Created by milena on 08/11/16.
//


#include <gtest/gtest.h>
#include <PersonManager.h>
#include "../Exceptions/UserAlreadyExistsException.h"

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
vector<Skill *> getSkillsDisponibles() {
    vector<Skill*> skills_disponibles;
    for (int i = 0; i < 5; i++) {
        Skill* skill = new Skill();
        skill->setName("skill" + std::to_string(i + 1));
        skills_disponibles.push_back(skill);
    }

    return skills_disponibles;
}

vector<WorkHistory *> getTrabajosDisponibles() {
    vector<WorkHistory*> trabajos_disponibles;
    for (int i = 0; i < 4; i++) {
        WorkHistory* workHistory = new WorkHistory();
        workHistory->setPositionTitle("jobPosition" + std::to_string(i + 1));
        trabajos_disponibles.push_back(workHistory);
    }
    return trabajos_disponibles;
}

void setDB() {

    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::string last_id = std::to_string(0);
    db->putKey("lastID", &last_id);
    RecommendationsManager* recommendationsManager = new RecommendationsManager(db);
    vector<long> users_id;
    vector<Person*> users;
    vector<Skill*> skills_disponibles = getSkillsDisponibles();
    vector<WorkHistory*> trabajos_disponibles = getTrabajosDisponibles();
    for (int i = 0; i < 5; i++) {
        Person* user = new Person();
        user->setId(0);
        user->setCity("Ciudad" + std::to_string(i));
        user->setDateOfBirth(std::to_string(rand() % 30 + 1) + "/" + std::to_string(rand() % 12 + 1) + "/1993");
        user->setEmail("usuariofalso" + std::to_string(i+1) + "@gmail.com");
        user->setLastName("ApellidoFalso" + std::to_string(i + 1));
        user->setFirstName("NombreFalso" + std::to_string(rand() % 100 + 1));
        user->setSummary("DescripcionFalsa" + std::to_string(i + 1));
        float x = -180.0;
        float y =  -180.0;
        user->setLocation(x, y);
        user->setPassword("usuarioFalso" + std::to_string(i + 1));

        users.push_back(user);

    }

    users[2]->addSkill(skills_disponibles[1]);
    users[2]->addSkill(skills_disponibles[2]);
    users[3]->addSkill(skills_disponibles[4]);
    users[4]->addSkill(skills_disponibles[2]);
    users[0]->addSkill(skills_disponibles[2]);

    users[0]->setTotalRecommendations(3);

    users[2]->setLocation(25.0,25.0);

    users[0]->addWorkHistory(trabajos_disponibles[0]);
    users[0]->addWorkHistory(trabajos_disponibles[1]);
    users[0]->addWorkHistory(trabajos_disponibles[2]);
    users[1]->addWorkHistory(trabajos_disponibles[1]);

    for (int i = 0; i < 5; i++) {
        try {
            long user_id = personManager->savePerson(users[i]->serializeMe());
            delete(users[i]);

        }catch(UserAlreadyExistsException& exception1) {}
    }


        delete personManager;
    delete recommendationsManager;
    std::vector<Skill*>::iterator it1 = skills_disponibles.begin();
    while (it1 != skills_disponibles.end()) {
        delete (*it1);
        it1 ++;
    }

    std::vector<WorkHistory*>::iterator it2 = trabajos_disponibles.begin();
    while (it2 != trabajos_disponibles.end()) {
        delete (*it2);
        it2 ++;
    }
}

TEST(SearchEngine, SearchByDistance) {
    setDB();
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("20.0");
    search1->push_back("20.0");
    search1->push_back("100.0");

    vector<Person*>* result = personManager->searchByDistance(search1);
    EXPECT_EQ(result->size(),1);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete search1;
    delete namedb;
    delete personManager;
}

TEST(SearchEngine, SearchByMail1) {
    setDB();
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("usuariofalso3");

    vector<Person*>* result = personManager->searchByMail(search1);
    EXPECT_EQ(result->size(),1);
    EXPECT_EQ((*result)[0]->getEmail().compare("usuariofalso3@gmail.com"),0);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete search1;
    delete namedb;
    delete personManager;
}


TEST(SearchEngine, SearchByMail2) {
    setDB();
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("usuariofalso9");

    vector<Person*>* result = personManager->searchByMail(search1);
    EXPECT_EQ(result->size(),0);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete search1;
    delete namedb;
    delete personManager;
}

TEST(SearchEngine, SearchByJobPosition) {
    setDB();
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("jobPosition2");

    vector<Person*>* result = personManager->searchByJobPosition(search1);
    EXPECT_EQ(result->size(),2);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete search1;
    delete namedb;
    delete personManager;
}

TEST(SearchEngine, SearchByJobPosition3) {
    setDB();
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("jobPositionss2");

    vector<Person*>* result = personManager->searchByJobPosition(search1);
    EXPECT_EQ(result->size(),0);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete search1;
    delete namedb;
    delete personManager;
}




TEST(SearchEngine, SearchByJobPosition2) {
    setDB();
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);
    std::vector<string>* search1 = new std::vector<string>();
    search1->push_back("jobPosition2");
    search1->push_back("jobPosition1");
    vector<Person*>* result = personManager->searchByJobPosition(search1);
    EXPECT_EQ(result->size(),1);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete search1;
    delete namedb;
    delete personManager;
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

