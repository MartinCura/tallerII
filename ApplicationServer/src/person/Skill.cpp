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
    this->name = name;
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
	this->name = jvalue["name"].asString();
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