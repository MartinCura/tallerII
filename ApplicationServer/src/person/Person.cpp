#include <algorithm>
#include "Person.h"

Person::Person() {
    this->id = 0;
    this->firstName = "";
    this->lastName = "" ;
    this->email = "";
    this->password = "";
    this->dateOfBirth = "";
    this->city = "";
    this->summary = "";
    this->totRecommendations = 0;
    this->location = new Location();
}

Person::Person(Json::Value personAsJson) {
    this->location = new Location();
    deserializeMe(personAsJson);
}

Person::~Person() {
    this->deleteWorkHistory();
    this->deleteSkills();
    delete this->location;
}

void Person::setId(long id) {
    this->id = id;
}

void Person::setFirstName(string firstName) {
    std::string aux = firstName;
    transform(aux.begin(), aux.end(), aux.begin(), ::tolower);
    this->firstName = firstName;
}

void Person::setLastName(string lastName) {
    std::string aux = lastName;
    transform(aux.begin(), aux.end(), aux.begin(), ::tolower);
    this->lastName = lastName;
}

void Person::setEmail(string email) {
    this->email = email;
}

void Person::setPassword(string password) {
    this->password = password;
}

void Person::setDateOfBirth(string dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}

void Person::setCity(string city) {
    transform(city.begin(), city.end(), city.begin(), ::tolower);
    this->city = city;
}

void Person::setSummary(string summary) {
    this->summary = summary;
}

void Person::setLocation(double latitude, double longitude) {
    this->location->setLatitude(latitude);
    this->location->setLongitude(longitude);
}

void Person::addWorkHistory(WorkHistory* workHistory) {
    this->workHistory.push_back(workHistory);
}

void Person::addSkill(Skill *skill){
    this->skills.push_back(skill);
}

long Person::getId() {
    return this->id;
}

string Person::getFirstName() {
    return this->firstName;
}

string Person::getLastName() {
    return this->lastName;
}

string Person::getFullName() {
    return this->firstName + "-" + this->lastName;

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

string Person::getSummary() {
    return this->summary;
}

Location* Person::getLocation() {
    return this->location;
}

vector<WorkHistory*> Person::getWorkHistory() {
    return this->workHistory;
}

vector<Skill*> Person::getSkills() {
    return this->skills;
}

string Person::getPassword() {
    return this->password;
}

WorkHistory* Person::getCurrentJob() {
    vector<WorkHistory*> workHistoryVector = this->getWorkHistory();
    for (vector<WorkHistory*>::size_type i = 0; i != workHistoryVector.size(); i++) {
        if (workHistoryVector[i]->getToDate() == "") {
            return workHistoryVector[i];
        }
    }
    return nullptr;
}

void Person::deserializeMe(Json::Value personAsJson) {
    this->id = personAsJson["id"].asLargestInt();
    this->firstName = personAsJson["first_name"].asString();
    std::transform(this->firstName.begin(), this->firstName.end(), this->firstName.begin(), ::tolower);
    this->lastName = personAsJson["last_name"].asString();
    std::transform(this->lastName.begin(), this->lastName.end(), this->lastName.begin(), ::tolower);
    this->email = personAsJson["email"].asString();
    std::transform(this->email.begin(), this->email.end(), this->email.begin(), ::tolower);
    this->password = personAsJson["password"].asString();
    this->dateOfBirth = personAsJson["date_of_birth"].asString();
    this->city = personAsJson["city"].asString();
    std::transform(this->city.begin(), this->city.end(), this->city.begin(), ::tolower);
    this->summary = personAsJson["summary"].asString();
    this->totRecommendations = personAsJson["tot_recommendations"].asInt();

    this->location->setLatitude(personAsJson["location"]["latitude"].asDouble());
    this->location->setLongitude(personAsJson["location"]["longitude"].asDouble());

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

void Person::updateMe(Json::Value values) {
    this->firstName = values["first_name"].asString();
    this->lastName = values["last_name"].asString();
    this->dateOfBirth = values["date_of_birth"].asString();
    this->city = values["city"].asString();
    this->summary = values["summary"].asString();

    this->location->setLatitude(values["location"]["latitude"].asDouble());
    this->location->setLongitude(values["location"]["longitude"].asDouble());

    this->deleteWorkHistory();
    if (values.isMember("work_history")) {
        const Json::Value jWorkHistoryVector = values["work_history"];
        for (int index = 0; index < jWorkHistoryVector.size(); index++) {
            WorkHistory *workHistory = new WorkHistory(jWorkHistoryVector[index]);
            this->addWorkHistory(workHistory);
        }
    }

    this->deleteSkills();
    if (values.isMember("skills")) {
        const Json::Value jSkillVector = values["skills"];
        for (int index2 = 0; index2 < jSkillVector.size(); index2++) {
            Skill *skill = new Skill(jSkillVector[index2]);
            this->addSkill(skill);
        }
    }
}

Json::Value Person::serializeMe() {
    Json::Value personAsJson;
    personAsJson["id"] = this->id;
    personAsJson["first_name"] = this->firstName;
    personAsJson["last_name"] = this->lastName;
    personAsJson["email"] = this->email;
    personAsJson["password"] = this->password;
    personAsJson["date_of_birth"] = this->dateOfBirth;
    personAsJson["city"] = this->city;
    personAsJson["summary"] = this->summary;
    personAsJson["location"]["latitude"] = this->location->getLatitude();
    personAsJson["location"]["longitude"] = this->location->getLongitude();
    personAsJson["tot_recommendations"] = this->totRecommendations;

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

void Person::deleteWorkHistory() {
    long size = this->workHistory.size();
    for (int i = 0; i != size; i++) {
        this->workHistory.pop_back();
    }
}

void Person::deleteSkills() {
    long size = this->skills.size();
    for (int i = 0; i != size; i++) {
        this->skills.pop_back();
    }
}

int Person::getTotalOfRecommendations() {
    return this->totRecommendations;
}

void Person::setTotalRecommendations(int i) {
    this->totRecommendations = i;
}

bool Person::has_every_skill(std::vector<string> *skills) {
    for(int i = 0; i < skills->size(); i++ ) {
        if (!this->hasSkill((*skills)[i])) return false;
    }
    return true;
}

bool Person::hasSkill(std::string skill) {
    for(int j = 0; j < this->skills.size(); j++) {
        if(this->skills[j]->getName().compare(skill) == 0) return true;
    }
    return false;
}

bool Person::has_every_position(std::vector<string> *positions) {
    for(int i = 0; i < positions->size(); i++ ) {
        if (!this->hasPosition((*positions)[i])) return false;
    }
    return true;
}

bool Person::hasPosition(std::string position) {
    for(int j = 0; j < this->workHistory.size(); j++) {
        std::string position2 = this->workHistory[j]->getPositionTitle();
        std::transform(position2.begin(), position2.end(), position2.begin(), ::tolower);
        if(position2.compare(position) == 0) return true;
    }
    return false;
}
