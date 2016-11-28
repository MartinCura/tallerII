//
// Created by milena on 10/11/16.
//

#include "Manager.h"
#include "../Exceptions/UserNotFoundException.h"
#include "../Exceptions/KeyNotFound.h"


void Manager::saveUser(Person *user) {
    Json::FastWriter fastWriter;
    string person_string = fastWriter.write(user->serializeMe());
    db->putKey(USER_MAIL_ID + user->getEmail(), &person_string);
}

bool Manager::userExistsMail(string user_mail) {
    string user_information;
    return db->existsKey(USER_MAIL_ID + user_mail, &user_information);
}

bool Manager::userExists(long user_id) {
    std::string result;
    return (db->existsKey(USER_UUID_ID + to_string(user_id), &result));
}

void Manager::saveUserNameKey(Person *user) {
    string user_mail = user->getEmail();
    db->putKey(USER_NAME_ID + user->getFullName() + "_" + user_mail, &user_mail);
}

void Manager::saveUserIdKey(Person *user) {
    string user_mail = user->getEmail();
    db->putKey(USER_UUID_ID + std::to_string(user->getId()), &user_mail);
}

void Manager::saveUserPasswordKey(Person *user, string user_password) {
    db->putKey(USER_PASSWORD + user->getEmail(), &user_password);
}

Person *Manager::getUserByMail(string user_mail) {
    string suser;
    Json::Value juser;
    if (!db->existsKey(USER_MAIL_ID + user_mail, &suser)) {
        throw UserNotFoundException(user_mail);
    }

    juser = this->getJsonFromString(suser);
    return new Person(juser);
}

void Manager::saveUserSkillKey(string skill_name, string value) {
    db->putKey(USER_SKILL + skill_name, &value);
}

void Manager::saveUserJobPositionKey(string job_position, string value) {
    db->putKey(USER_JOBPOSITION + job_position, &value);
}

Json::Value Manager::getJsonFromString(std::string svalue) {
    Json::Reader reader;
    Json::Value jvalue;
    reader.parse(svalue.c_str(), jvalue);
    return  jvalue;
}

void Manager::deleteUserIdKey(Person *user) {
    db->deleteKey(USER_UUID_ID + std::to_string(user->getId()));
}

void Manager::deleteUser(Person *user) {
    db->deleteKey(USER_MAIL_ID + user->getEmail());
}

Person* Manager::getUserById(long id) {

    std::string  user_mail;
    if (db->existsKey(USER_UUID_ID + std::to_string(id), &user_mail)) {
        return getUserByMail(user_mail);
    } else {
        //No se encontro el usuario
        throw UserNotFoundException(id);
    }
}

void Manager::deleteUserNameKey(Person *user) {
    db->deleteKey(USER_NAME_ID + user->getFullName() + "_" + user->getEmail());
}

void Manager::deleteUserPasswordKey(Person* user) {
    db->deleteKey(USER_PASSWORD + user->getEmail());
}

string Manager::getUserSkillKey(string skill){
    string users_withSkill;
    if (!db->existsKey(USER_SKILL + skill, &users_withSkill)) throw KeyNotFound();
    return users_withSkill;
}

string Manager::getUserJobPositionKey(string job_position){
    string users_withSkill;
    if (!db->existsKey(USER_JOBPOSITION + job_position, &users_withSkill)) throw KeyNotFound();
    return users_withSkill;
}

string Manager::getUserIdKey(long id) {
    string user_mail;
    if (!db->existsKey(USER_UUID_ID + std::to_string(id), &user_mail)) throw UserNotFoundException(id);
    return user_mail;
}

