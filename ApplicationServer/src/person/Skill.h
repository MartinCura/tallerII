#ifndef PROJECT_SKILL_H
#define PROJECT_SKILL_H

#include <string>
#include "json/json.h"
#include "json/json-forwards.h"

using namespace std;

class Skill {

public:
    Skill();
    Skill(Json::Value jvalue);
    virtual ~Skill();

    void setName(string name);
    void setDescription(string description);
    void setCategory(string category);

    string getName();
    string getDescription();
    string getCategory();

    Json::Value serializeMe();

private:
    string name;
    string description;
    string category;

    void deserializeMe(Json::Value jvalue);
};

#endif //PROJECT_SKILL_H
