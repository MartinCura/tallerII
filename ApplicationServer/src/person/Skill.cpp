#include <algorithm>
#include "Skill.h"

Skill::Skill() {
    this->name = "";
    this->description = "";
    this->category = "";
}

Skill::Skill(Json::Value jvalue) {
	this->deserializeMe(jvalue);
}
Skill::~Skill() {}

void Skill::setName(string name) {
    std::string name2 = name;
    transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
    this->name = name2;
}

void Skill::setDescription(string description) {
    this->description = description;
}

void Skill::setCategory(string category) {
    this->category = category;
}

string Skill::getName() {
    return this->name;
}

string Skill::getDescription() {
    return this->description;
}

string Skill::getCategory() {
    return this->category;
}

void Skill::deserializeMe(Json::Value jvalue) {
    std::string skill_name = jvalue["name"].asString();
    transform(skill_name.begin(), skill_name.end(), skill_name.begin(), ::tolower);
	this->name = skill_name;
    this->description = jvalue["description"].asString();
    this->category = jvalue["category"].asString();
}

Json::Value Skill::serializeMe() {
	Json::Value response;
    response["name"] = this->name;
    response["description"] = this->description;
    response["category"] = this->category;

    return response;
}