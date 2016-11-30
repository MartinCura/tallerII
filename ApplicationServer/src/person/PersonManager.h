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
#include "Manager.h"

class PersonManager : public Manager{

public:
    PersonManager(DBWrapper *db);
    virtual ~PersonManager();


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
    long savePerson(Json::Value juser_new_information, long forceID = -1);

    void deletePerson(long id);
    vector<Contact*> getContactsByUserId(long id);
    void saveOrUpdateRelation(Json::Value relation);
    void saveRecommendation(Json::Value recommendation);
    void removeRecommendation(long fromUserId, long toUserId);
    Json::Value getRecommendationsByUserId(long userId);
    string saveMessage(Json::Value request);
    vector<Message*> getMessages(long fromUserId, long toUserId);
    string getNotificationTokenByUserId(long userId);
    void setOrUpdateNotificationToken(Json::Value request, long userId);
    vector<Person *> *searchByName(vector<string> *user_searchName);
    long updateUser(Json::Value juser_new_information);
    vector<Person*> *getAllUsers();
    vector<Person *> * searchBySkill(vector<string> *skills_search);
    vector<Person *> *searchByJobPosition(vector<string> *job_positions);
    vector<Person *> *searchByMail(vector<string> *user_mail);


    vector<Person *> *search_users_by(map<string, vector<string>*> *search_values);

private:
    long generateID();
    void validateUsersOfRequest(long fromUserId, long toUserId);
    void saveSkills(std::vector<Skill *> user_newSkills, string user_mail);
    void updateName(Person* new_user, Person* old_user);
    void saveSkill(Skill *skill, string user_mail);
    void updateSkills(Person *new_user, Person *old_user);
    void deleteUserFromSkill(string skill_name, string user_mail);
    void savePositions(vector<WorkHistory *> positions, string user_mail);
    void savePosition(string position_title, string user_mail);
    void deleteUserFromJobPosition(string job_position, string user_mail);
    void updateWorkHistory(vector<WorkHistory *> new_positions, vector<WorkHistory *> old_positions, string user_mail);

    vector<Person *> *searchByDistance(vector<string> *distance_search);


    void split2(const string &s, char delim, vector<string> *elems);

    vector<string> *split(const string &s, char delim);
};

#endif //PROJECT_PERSONMANAGER_H
