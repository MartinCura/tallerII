#include "Person.h"

Person::Person() {
    this->firstName = "";
    this->lastName = "" ;
    this->email = "";
    this->dateOfBirth = "";
    this->city = "";
    this->profilePicture = "";
    this->summary = "";
}

Person::~Person() {
    vector<WorkHistory*> workHistoryVector = this->getWorkHistory();
    for (vector<WorkHistory*>::size_type i = 0; i != workHistoryVector.size(); i++) {
        delete workHistoryVector[i];
    }
    vector<Skill*> skillsVector = this->getSkills();
    for (vector<Skill*>::size_type i = 0; i != skillsVector.size(); i++) {
        delete skillsVector[i];
    }
}

void Person::setFirstName(string firstName) {
    this->firstName = firstName;
}

void Person::setLastName(string lastName) {
    this->lastName = lastName;
}

void Person::setEmail(string email) {
    this->email = email;
}

void Person::setDateOfBirth(string dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}

void Person::setCity(string city) {
    this->city = city;
}

void Person::setProfilePicture(string profilePicture) {
    this->profilePicture = profilePicture;
}

void Person::setSummary(string summary) {
    this->summary = summary;
}

void Person::addWorkHistory(WorkHistory* workHistory) {
    this->workHistory.push_back(workHistory);
}

void Person::addSkill(Skill *skill){
    this->skills.push_back(skill);
}

string Person::getFirstName() {
    return this->firstName;
}

string Person::getLastName() {
    return this->lastName;
}

string Person::getEmail() {
    return this->email;
}

string Person::getDateOfBirth() {
    return this->dateOfBirth;
}

string Person::getCity() {
    return this->city;
}

string Person::getProfilePicture() {
    return this->profilePicture;
}

string Person::getSummary() {
    return this->summary;
}

vector<WorkHistory*> Person::getWorkHistory() {
    return this->workHistory;
}

vector<Skill*> Person::getSkills() {
    return this->skills;
}
