#ifndef PROJECT_PERSON_H
#define PROJECT_PERSON_H

#include <string>
#include <vector>
#include "WorkHistory.h"
#include "Skill.h"
#include "json/json.h"
#include "json/json-forwards.h"

using namespace std;

class Person {

public:

    Person(Json::Value);
    Person();

    virtual ~Person();

    void setFirstName(string firstName);
    void setLastName(string lastName);
    void setEmail(string email);
    void setDateOfBirth(string dateOfBirth);
    void setCity(string city);
    void setProfilePicture(string profilePicture);
    void setSummary(string summary);
    void addWorkHistory(WorkHistory* workHistory);
    void addSkill(Skill* skill);

    string getFirstName();
    string getLastName();
    string getEmail();
    string getDateOfBirth();
    string getCity();
    string getProfilePicture();
    string getSummary();
    vector<WorkHistory*> getWorkHistory();
    vector<Skill*> getSkills();

    /// Converts the object Person into a Json
    Json::Value serializeMe();


private:
    string firstName;
    string lastName;
    string email;
    string dateOfBirth;
    string city;
    string profilePicture;
    string summary;
    vector<WorkHistory*> workHistory;
    vector<Skill*> skills;


    /// Creates an object Person from a Json
    Person desserializeMe(Json::Value jvalue);
};

#endif //PROJECT_PERSON_H
