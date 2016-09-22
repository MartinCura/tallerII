#include "Person.h"
#include <iostream>

Person::Person() {
    this->id = 0;
    this->firstName = "";
    this->lastName = "" ;
    this->email = "";
    this->dateOfBirth = "";
    this->city = "";
    this->profilePicture = "";
    this->summary = "";
    //this->workHistoryVector = new 
}

Person::Person(Json::Value jvalue) {

    deserializeMe(jvalue);
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

void Person::deserializeMe(Json::Value jvalue) {

    this->id = jvalue["id"].asInt();
    this->firstName = jvalue["first_name"].asString();
    this->lastName = jvalue["last_name"].asString();
    this->email = jvalue["email"].asString();
    this->dateOfBirth = jvalue["date_of_birth"].asString();
    this->city = jvalue["city"].asString();
    this->profilePicture = "";
    this->summary = jvalue["summary"].asString();

//     const Json::Value my_plugins = root["my-plug-ins"];
// for ( int index = 0; index < my_plugins.size(); ++index )  // Iterates over the sequence elements.
//    yourlib::loadPlugIn( my_plugins[index].asString() );
    //TODO: Missing WorkHistory

    const Json::Value jWorkHistoryVector = jvalue["work_history"];
    for (int index = 0; index < jWorkHistoryVector.size(); index++) {
        //Iterates over the sequence elements.
        WorkHistory* workHistory = new WorkHistory(jWorkHistoryVector[index]);
        this->addWorkHistory(workHistory);
    }

    const Json::Value jSkillVector = jvalue["skills"];
    for (int index2 = 0; index2 < jSkillVector.size(); index2++) {
        //Iterates over the sequence elements.
        Skill* skill = new Skill(jSkillVector[index2]);
        this->addSkill(skill);
    }



}

Json::Value Person::serializeMe() {

    Json::Value response;
    response["id"] = this->id;
    response["first_name"] = this->firstName;
    response["last_name"] = this->lastName;
    response["email"] = this->email;
    response["date_of_birth"] = this->dateOfBirth;
    response["city"] = this->city;
    response["profile_picture"] = this->profilePicture;
    response["summary"] = this->summary;

    for (vector<WorkHistory*>::size_type i = 0; i != this->workHistory.size(); i++) {
        WorkHistory* workHistory = this->workHistory[i];
        Json::Value workHistoryResponse = workHistory->serializeMe();
        response["work_history"].append(workHistoryResponse);
    }

    for (vector<Skill*>::size_type i = 0; i != this->skills.size(); i++) {
        Skill* skill = this->skills[i];
        Json::Value skillsResponse = skill->serializeMe();
        response["skills"].append(skillsResponse);
    }

    return response;
}
