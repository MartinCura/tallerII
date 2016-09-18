#include "Skill.h"

Skill::Skill() {
    this->name = "";
    this->description = "";
    this->category = "";
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
