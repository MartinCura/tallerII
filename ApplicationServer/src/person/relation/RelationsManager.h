#ifndef PROJECT_RELATIONSMANAGER_H
#define PROJECT_RELATIONSMANAGER_H

#include "../../DB/DBWrapper.h"
#include "../../Exceptions/ErrorOpeningDatabaseException.h"
#include "../../Exceptions/InvalidRequestException.h"
#include "Contact.h"

using namespace std;

class RelationsManager {

public:
    static const string CONTACT_KEY;
    static const string ACTIVE;
    static const string RECEIVED;
    static const string REQUESTED;
    static const string ADD_CONTACT_ACTION;
    static const string ACCEPT_CONTACT_ACTION;
    static const string REMOVE_CONTACT_ACTION;

    RelationsManager(DBWrapper* db);
    ~RelationsManager();

    void saveRelation(long authorId, long contactId, string action);
    vector<Contact*> getContactsByUserId(long userId);

private:
    DBWrapper* db;

    void addOrUpdateRelation(long keyUserId, long contactId, string status);
    void removeRelation(long keyUserId, long contactId);
};

#endif //PROJECT_RELATIONSMANAGER_H
