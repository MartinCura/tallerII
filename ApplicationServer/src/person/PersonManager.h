#ifndef PROJECT_PERSONMANAGER_H
#define PROJECT_PERSONMANAGER_H

#include <leveldb/db.h>
#include "Person.h"
#include "../Exceptions/UserNotFoundException.h"
#include "../DB/DBWrapper.h"
#include "../Exceptions/InvalidRequestException.h"
#include "../Exceptions/ErrorOpeningDatabaseException.h"
#include "relation/RelationsManager.h"
#include "relation/Contact.h"
#include "recommendation/RecommendationsManager.h"
#include "messages/MessagesManager.h"
#include "messages/NotificationTokenManager.h"

class PersonManager {

public:
    PersonManager(DBWrapper *db);
    virtual ~PersonManager();

    Person* getPersonById(long id);
    Person* getUserByMail(string *user_mail);


    /*
     * Guarda un usuario nuevo.
     * El usuario para ser considerado "nuevo" no debe poseer id.
     * Se puede forzar el ingreso de un contacto dandole valor a <forceID>
     * Tambien guarda las ediciones de un contacto a su perfil.
     * Devuelve en ambos casos el id que identifica al usuario.
     * En la base de datos, guarda la informacion con la siguiente estructura.
     * USER_MAIL_ID + <user_mail> = <json_usuario>
     * USER_UUID_ID + <user_id> = <user_mail>
     * USER_PASSWORD + <user_mail> = <password>
     */
    long savePerson(Json::Value juser_new_information, long forceID);

    void deletePerson(long id);
    vector<Contact*> getContactsByUserId(long id);
    void saveOrUpdateRelation(Json::Value relation);
    void saveRecommendation(Json::Value recommendation);
    void removeRecommendation(long fromUserId, long toUserId);
    Json::Value getRecommendationsByUserId(long userId);
    string saveMessage(Json::Value request);
    vector<Message*> getMessages(long fromUserId, long toUserId);
    bool userExists(long userId);
    string getNotificationTokenByUserId(long userId);
    void setOrUpdateNotificationToken(Json::Value request, long userId);
    vector<Person *> *searchByName(std::string user_searchName);
    long updateUser(Json::Value juser_new_information);
    vector<Person*> *getAllUsers();
    vector<Person *> * searchBySkill(string skill);
    vector<Person *> *searchByJobPosition(string job_position);
    vector<Person *> *searchByMail(string user_mail);

private:
    DBWrapper* db;

    long generateID();
    void validateUsersOfRequest(long fromUserId, long toUserId);
    Json::Value getJsonFromString(string svalue);
    void saveSkills(std::vector<Skill *> user_newSkills, string user_mail);
    void updateName(string user_newName, string user_oldName, string user_mail);
    void saveSkill(string skill_name, string user_mail);
    void updateSkills(vector<Skill *> new_skills, vector<Skill *> old_skills, string user_mail);
    void deleteUserFromSkill(string skill_name, string user_mail);


    void savePositions(vector<WorkHistory *> positions, string user_mail);

    void savePosition(string position_title, string user_mail);

    void deleteUserFromJobPosition(string job_position, string user_mail);

    void updateWorkHistory(vector<WorkHistory *> new_positions, vector<WorkHistory *> old_positions, string user_mail);
};

#endif //PROJECT_PERSONMANAGER_H
