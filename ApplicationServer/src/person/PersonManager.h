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
#include "messages/Conversation.h"
#include "Manager.h"

class PersonManager : public Manager{

public:
    PersonManager(DBWrapper *db);
    virtual ~PersonManager();


    long savePerson(Json::Value juser_new_information, long forceID = -1);
    void deletePerson(long id);
    vector<Contact*> getContactsByUserId(long id);
    void saveOrUpdateRelation(Json::Value relation);
    void saveRecommendation(Json::Value recommendation);
    void removeRecommendation(long fromUserId, long toUserId);
    Json::Value getRecommendationsByUserId(long userId);
    string saveMessage(Json::Value request);
    vector<Message*> getMessages(long fromUserId, long toUserId);
    long updateUser(Json::Value juser_new_information);
    vector<Conversation*> getAllConversations(long userId);
    void setMessagesAsReceived(long fromUserId, long toUserId, vector<Message*> deliveredMessages);
    string getNotificationTokenByUserId(long userId);
    void setOrUpdateNotificationToken(Json::Value request, long userId);
    vector<Person*> *getAllUsers();
    vector<Person *> *searchByName(vector<string> *user_searchName);
    vector<Person *> * searchBySkill(vector<string> *skills_search);
    vector<Person *> *searchByJobPosition(vector<string> *job_positions);
    vector<Person *> *searchByMail(vector<string> *user_mail);
    vector<Person *> *search_users_by(map<string, vector<string>*> *search_values);
    vector<Person *> *searchByDistance(vector<string> *distance_search);


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
    void updateWorkHistory(Person *new_user, Person *old_user);


    void split2(const string &s, char delim, vector<string> *elems);
    vector<string> *split(const string &s, char delim);
    void updatePassword(Person *new_user, Person *old_user);

    vector<long> *getAllUsersIds();
};

#endif //PROJECT_PERSONMANAGER_H
