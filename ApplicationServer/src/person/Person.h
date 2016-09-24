#ifndef PROJECT_PERSON_H
#define PROJECT_PERSON_H

#include <string>
#include <vector>
#include <iostream>
#include "WorkHistory.h"
#include "Skill.h"
#include "../jsoncpp/json/json.h"
#include "../jsoncpp/json/json-forwards.h"

using namespace std;

class Person {

public:

    Person(Json::Value);
    Person();
    ~Person();

    void setId(int id);
    void setFirstName(string firstName);
    void setLastName(string lastName);
    void setEmail(string email);
    void setDateOfBirth(string dateOfBirth);
    void setCity(string city);
    void setProfilePicture(string profilePicture);
    void setSummary(string summary);
    void addWorkHistory(WorkHistory* workHistory);
    void addSkill(Skill* skill);

    int getId();
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
    int id;
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
    void deserializeMe(Json::Value personAsJson);
};

#endif //PROJECT_PERSON_H
