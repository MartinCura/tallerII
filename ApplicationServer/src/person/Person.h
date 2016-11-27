#ifndef PROJECT_PERSON_H
#define PROJECT_PERSON_H

#include <string>
#include <vector>
#include <iostream>
#include "WorkHistory.h"
#include "Skill.h"
#include "Location.h"
#include "../jsoncpp/json/json.h"
#include "../jsoncpp/json/json-forwards.h"

using namespace std;

class Person {

public:

    Person(Json::Value);
    Person();
    ~Person();

    void setId(long id);
    void setFirstName(string firstName);
    void setLastName(string lastName);
    void setEmail(string email);
    void setPassword(string password);
    void setDateOfBirth(string dateOfBirth);
    void setCity(string city);
    void setSummary(string summary);
    void setLocation(double latitude, double longitude);
    void addWorkHistory(WorkHistory* workHistory);
    void addSkill(Skill* skill);

    long getId();
    string getFirstName();
    string getLastName();
    string getEmail();
    string getDateOfBirth();
    string getCity();
    string getSummary();
    Location* getLocation();
    vector<WorkHistory*> getWorkHistory();
    vector<Skill*> getSkills();
    WorkHistory* getCurrentJob();

    /// Converts the object Person into a Json
    Json::Value serializeMe();
    void updateMe(Json::Value values);

    string getFullName();

    int getTotalOfRecommendations();
    void setTotalRecommendations(int i);
    string getPassword();

private:
    long id;
    int totRecommendations;
    string firstName;
    string lastName;
    string email;
    string password;
    string dateOfBirth;
    string city;
    string summary;
    Location* location;
    vector<WorkHistory*> workHistory;
    vector<Skill*> skills;

    /// Creates an object Person from a Json
    void deserializeMe(Json::Value personAsJson);
    void deleteWorkHistory();
    void deleteSkills();



};

#endif //PROJECT_PERSON_H
