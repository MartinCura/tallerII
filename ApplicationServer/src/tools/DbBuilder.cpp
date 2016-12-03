#include <algorithm>
#include "DbBuilder.h"
#include "../session/SessionManager.h"

const char *JOB_POSITIONS_FILE = "../ApplicationServer/src/tools/DbBuilderSettings/JobPositionsFromSharedServer.js";
const char *SKILLS_FILE = "../ApplicationServer/src/tools/DbBuilderSettings/SkillsFromSharedServer.js";

DbBuilder::DbBuilder() {
    this->namedb = new string(Config::getInstance()->get(Config::NAME_DB));
    db = DBWrapper::openDb(namedb);
    this->loadAvailableJobPositions();
    this->loadAvailableSkills();
}

DbBuilder::~DbBuilder() {
    DBWrapper::ResetInstance();
    delete namedb;

    long sizePos = this->jobPositions.size();
    for (int i = 0; i != sizePos; i++) {
        this->jobPositions.pop_back();
    }

    long sizeSkills = this->skills.size();
    for (int i = 0; i != sizeSkills; i++) {
        this->skills.pop_back();
    }
}

void DbBuilder::loadUsers() {
    setLastId();
    PersonManager *personManager = new PersonManager(this->db);
    SessionManager* sessionManager = new SessionManager(this->db);
    try {
        Person* person1 = this->getFakePerson1();
        personManager->savePerson(person1->serializeMe(), (long) 1);
        sessionManager->saveToken("tokenUser1", person1->getEmail());
        delete person1;
    } catch (UserAlreadyExistsException &exception) {}
    try {
        Person* person2 = this->getFakePerson2();
        personManager->savePerson(person2->serializeMe(), (long) 2);
        sessionManager->saveToken("tokenUser2", person2->getEmail());
        delete person2;
    } catch (UserAlreadyExistsException &exception) {}
    try {
        Person* person3 = this->getFakePerson3();
        personManager->savePerson(person3->serializeMe(), (long) 3);
        sessionManager->saveToken("tokenUser3", person3->getEmail());
        delete person3;
    } catch (UserAlreadyExistsException &exception) {}
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

    person->addWorkHistory(this->getWorkHistory(0, "IBM", "10/2012", "11/2014"));
    person->addWorkHistory(this->getWorkHistory(1, "Amazon", "12/2014", ""));
    person->addSkill(this->getSkill(0));
    person->addSkill(this->getSkill(1));

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

    person->addWorkHistory(this->getWorkHistory(2, "Microsoft", "10/2013", "11/2015"));
    person->addWorkHistory(this->getWorkHistory(3, "E-bay", "12/2015", ""));
    person->addSkill(this->getSkill(2));
    person->addSkill(this->getSkill(3));

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

    person->addWorkHistory(this->getWorkHistory(2, "Microsoft", "10/2012", "11/2014"));
    person->addSkill(this->getSkill(2));

    return person;
}

WorkHistory* DbBuilder::getWorkHistory(int jobPositionIndex, string company, string fromDate, string toDate) {
    WorkHistory* workHistory = new WorkHistory();
    workHistory->setCompany(company);
    workHistory->setPositionTitle(this->jobPositions[jobPositionIndex]);
    workHistory->setFromDate(fromDate);
    workHistory->setToDate(toDate);
    return workHistory;
}

Skill* DbBuilder::getSkill(int skillIndex) {
    Skill* skill = new Skill();
    skill->setName(this->skills[skillIndex]->getName());
    skill->setDescription(this->skills[skillIndex]->getDescription());
    skill->setCategory(this->skills[skillIndex]->getCategory());
    return skill;
}

void DbBuilder::loadAvailableJobPositions() {
    string jobPositionsAsString = this->loadFile(JOB_POSITIONS_FILE);
    Json::Value jobPositionsAsArray = this->parseFile(jobPositionsAsString)["job_positions"];
    for (int index = 0; index < jobPositionsAsArray.size(); index++) {
        this->jobPositions.push_back(jobPositionsAsArray[index]["name"].asString());
    }
}

void DbBuilder::loadAvailableSkills() {
    string skillsAsString = this->loadFile(SKILLS_FILE);
    Json::Value skillsAsArray = this->parseFile(skillsAsString)["skills"];
    for (int index = 0; index < skillsAsArray.size(); index++) {
        Skill* skill = new Skill(skillsAsArray[index]);
        this->skills.push_back(skill);
    }
}

void DbBuilder::setLastId(){
    std::string last_id = std::to_string(3);
    db->putKey("lastID", &last_id);
}

string DbBuilder::loadFile(string path) {
    string line;
    string fileContent;
    ifstream settingsFile(path.c_str());
    if (!settingsFile.is_open()) {
        throw "Invalid or nonexistent file";
    }
    while (getline(settingsFile, line)) {
        fileContent += line;
    }
    settingsFile.close();
    return fileContent;
}

Json::Value DbBuilder::parseFile(string fileContent) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(fileContent, root);
    if (!parsingSuccessful) {
        throw "Invalid Config file.";
    }
    return root;
}
