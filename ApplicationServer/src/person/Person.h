#ifndef PROJECT_PERSON_H
#define PROJECT_PERSON_H

#include <string>
#include <vector>
#include "WorkHistory.h"

using namespace std;

class Person {

public:
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

    string getFirstName();
    string getLastName();
    string getEmail();
    string getDateOfBirth();
    string getCity();
    string getProfilePicture();
    string getSummary();
    vector<WorkHistory*> getWorkHistory();

private:
    string firstName;
    string lastName;
    string email;
    string dateOfBirth;
    string city;
    string profilePicture;
    string summary;
    vector<WorkHistory*> workHistory;
};

#endif //PROJECT_PERSON_H
