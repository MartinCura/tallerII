#ifndef PROJECT_MESSAGESMANAGER_H
#define PROJECT_MESSAGESMANAGER_H

#include "../../DB/DBWrapper.h"
#include "../../Exceptions/ErrorOpeningDatabaseException.h"
#include "../../Exceptions/InvalidRequestException.h"
#include "Message.h"

using namespace std;

class MessagesManager {

public:
    MessagesManager(DBWrapper* db);
    ~MessagesManager();

    string saveMessage(long fromUserId, long toUserId, string message);
    vector<Message*> getMessages(long fromUserId, long toUserId);
    void setMessagesAsReceived(long fromUserId, long toUserId, vector<Message*> deliveredMessages);

private:
    DBWrapper* db;
    string getTimestamp();
    string getKey(long fromUserId, long toUserId);
};

#endif //PROJECT_MESSAGESMANAGER_H
