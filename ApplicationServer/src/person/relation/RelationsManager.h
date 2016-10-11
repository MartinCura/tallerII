#ifndef PROJECT_RELATIONSMANAGER_H
#define PROJECT_RELATIONSMANAGER_H

#include "../../DB/DBWrapper.h"
#include "../../Exceptions/ErrorOpeningDatabaseException.h"
#include "../../Exceptions/InvalidRequestException.h"
#include "Contact.h"

using namespace std;

class RelationsManager {

public:
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
