#ifndef PROJECT_DBBUILDER_H
#define PROJECT_DBBUILDER_H

#include "../person/Person.h"
#include "../person/PersonManager.h"
#include "../logger/Logger.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include "../config/Config.h"

using namespace std;

class DbBuilder {

public:
    DbBuilder();
    ~DbBuilder();

    void loadUsers();
    Person* getFakePerson1();
    Person* getFakePerson2();
    Person* getFakePerson3();
    Person* getFakePerson4();
    Person* getFakePerson5();

private:
    DBWrapper* db;
    string *namedb;
    vector<string> jobPositions;
    vector<Skill*> skills;

    void setLastId();
    string loadFile(string path);
    Json::Value parseFile(string fileContent);
    void loadAvailableJobPositions();
    void loadAvailableSkills();
    WorkHistory* getWorkHistory(int jobPositionIndex, string company, string fromDate, string toDate);
    Skill* getSkill(int skillIndex);
};

#endif //PROJECT_DBBUILDER_H
