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
    this->personAsJson = "";
    this->location = new Location();
}

Person::Person(Json::Value personAsJson) {
    this->location = new Location();
    deserializeMe(personAsJson);
    this->personAsJson = personAsJson;
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
    delete this->location;
}

void Person::setId(long id) {
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

void Person::setPassword(string password) {
    this->password = password;
}

void Person::setDateOfBirth(string dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}

void Person::setCity(string city) {
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
    this->lastName = personAsJson["last_name"].asString();
    this->email = personAsJson["email"].asString();
    this->password = personAsJson["password"].asString();
    this->dateOfBirth = personAsJson["date_of_birth"].asString();
    this->city = personAsJson["city"].asString();
    this->summary = personAsJson["summary"].asString();

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

Json::Value Person::serializeMe() {
    if (this->personAsJson != "") {
        return this->personAsJson;
    }
    Json::Value personAsJson;
    personAsJson["id"] = this->id;
    personAsJson["first_name"] = this->firstName;
    personAsJson["last_name"] = this->lastName;
    personAsJson["email"] = this->email;
    personAsJson["date_of_birth"] = this->dateOfBirth;
    personAsJson["city"] = this->city;
    personAsJson["summary"] = this->summary;
    personAsJson["location"]["latitude"] = this->location->getLatitude();
    personAsJson["location"]["longitude"] = this->location->getLongitude();

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
