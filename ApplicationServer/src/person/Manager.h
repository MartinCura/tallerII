//
// Created by milena on 10/11/16.
//

#ifndef APPLICATIONSERVER_MANAGER_H
#define APPLICATIONSERVER_MANAGER_H


#include "Person.h"
#include "../DB/DBWrapper.h"

#define USER_MAIL_ID "user:mail_"
#define USER_UUID_ID "user:uuid_"
#define USER_PASSWORD "user:password_"
#define USER_NAME_ID "user:name_"
#define USER_SKILL "user:skill_"
#define USER_JOBPOSITION "user:jobPosition_"

class Manager {

public:
    Person *getUserById(long id);
    Person* getUserByMail(string user_mail);
    bool userExistsMail(string user_mail);
    bool userExists(long user_id);


protected:
    DBWrapper* db;


    void saveUser(Person* user);
    void saveUserNameKey(Person *user);
    void saveUserIdKey(Person *user);
    void saveUserPasswordKey(Person *user);


    string getUserSkillKey(string skill);
    Json::Value getJsonFromString(string svalue);

    void deleteUserIdKey(Person *user);
    void deleteUserNameKey(Person *user);
    void deleteUserPasswordKey(Person *user);
    void deleteUser(Person *user);

    string getUserIdKey(long id);

    void saveUserSkillKey(string skill_name, string value);

    string getUserJobPositionKey(string job_position);

    void saveUserJobPositionKey(string job_position, string value);

    void deleteUserSkillKey(string skill_name);
};


#endif //APPLICATIONSERVER_MANAGER_H
