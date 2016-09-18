#ifndef PROJECT_SKILL_H
#define PROJECT_SKILL_H

#include <string>

using namespace std;

class Skill {

public:
    Skill();
    virtual ~Skill();

    void setName(string name);
    void setDescription(string description);
    void setCategory(string category);

    string getName();
    string getDescription();
    string getCategory();

private:
    string name;
    string description;
    string category;
};

#endif //PROJECT_SKILL_H
