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

class PersonManager {

public:
    PersonManager(std::string nameDB);
    virtual ~PersonManager();

    Person* getPersonById(long id);
    Person* getPersonByMail(string* user_mail);
    void destroyDB();

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
    long savePerson(long user_id, Json::Value person_json, long forceID = -1);
    vector<long> * getAllUsersIds();
    void deletePerson(long id);
    vector<Contact*> getContactsByUserId(long id);
    void saveOrUpdateRelation(Json::Value relation);
    void saveRecommendation(Json::Value recommendation);
    void removeRecommendation(long fromUserId, long toUserId);
    Json::Value getRecommendationsByUserId(long userId);
    void saveMessage(Json::Value request);
    vector<Message*> getMessages(long fromUserId, long toUserId);
    bool userExists(long userId);
private:
    DBWrapper* db;
    std::string nameDB;

    long generateID();
    void validateParametersOfRequest(Json::Value request);
    void validateUsersOfRequest(long fromUserId, long toUserId);
};

#endif //PROJECT_PERSONMANAGER_H
