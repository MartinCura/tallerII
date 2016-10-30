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

class PersonManager {

public:
    PersonManager(std::string nameDB);
    virtual ~PersonManager();

    Person* getPersonById(long id);
    Person* getPersonByMail(string* user_mail);
    void destroyDB();

    long savePerson(Json::Value person_json, long forceID = -1);
    vector<long> * getAllUsersIds();
    void deletePerson(long id);
    vector<Contact*> getContactsByUserId(long id);
    void saveOrUpdateRelation(Json::Value relation);
    void saveRecommendation(Json::Value recommendation);
    void removeRecommendation(Json::Value recommendation);
    Json::Value getRecommendationsByUserId(long userId);
    void login(string user_mail, string user_password);
    bool userExists(long userId);
private:
    DBWrapper* db;
    std::string nameDB;

    long generateID();
    void validateParametersOfRecommendationRequest(Json::Value recommendation);
    void validateUsersOfRecommendationRequest(long fromUserId, long toUserId);
};

#endif //PROJECT_PERSONMANAGER_H
