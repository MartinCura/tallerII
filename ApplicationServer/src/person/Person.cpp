#include "Person.h"

Person::Person() {
    this->id = 0;
    this->firstName = "";
    this->lastName = "" ;
    this->email = "";
    this->dateOfBirth = "";
    this->city = "";
    this->profilePicture = "";
    this->summary = "";
}

Person::Person(Json::Value personAsJson) {
    deserializeMe(personAsJson);
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

void Person::setId(int id) {
    this->id = id;
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

int Person::getId() {
    return this->id;
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

void Person::deserializeMe(Json::Value personAsJson) {
    this->id = personAsJson["id"].asInt();
    this->firstName = personAsJson["first_name"].asString();
    this->lastName = personAsJson["last_name"].asString();
    this->email = personAsJson["email"].asString();
    this->dateOfBirth = personAsJson["date_of_birth"].asString();
    this->city = personAsJson["city"].asString();
    this->profilePicture = "";
    this->summary = personAsJson["summary"].asString();

    const Json::Value jWorkHistoryVector = personAsJson["work_history"];
    for (int index = 0; index < jWorkHistoryVector.size(); index++) {
        WorkHistory* workHistory = new WorkHistory(jWorkHistoryVector[index]);
        this->addWorkHistory(workHistory);
    }

    const Json::Value jSkillVector = personAsJson["skills"];
    for (int index2 = 0; index2 < jSkillVector.size(); index2++) {
        Skill* skill = new Skill(jSkillVector[index2]);
        this->addSkill(skill);
    }
}

Json::Value Person::serializeMe() {
    Json::Value personAsJson;
    personAsJson["id"] = this->id;
    personAsJson["first_name"] = this->firstName;
    personAsJson["last_name"] = this->lastName;
    personAsJson["email"] = this->email;
    personAsJson["date_of_birth"] = this->dateOfBirth;
    personAsJson["city"] = this->city;
    personAsJson["profile_picture"] = this->profilePicture;
    personAsJson["summary"] = this->summary;
    for (vector<WorkHistory*>::size_type i = 0; i != this->workHistory.size(); i++) {
        WorkHistory* workHistory = this->workHistory[i];
        Json::Value workHistoryResponse = workHistory->serializeMe();
        personAsJson["work_history"].append(workHistoryResponse);
    }
    for (vector<Skill*>::size_type i = 0; i != this->skills.size(); i++) {
        Skill* skill = this->skills[i];
        Json::Value skillsResponse = skill->serializeMe();
        personAsJson["skills"].append(skillsResponse);
    }
    return personAsJson;
}