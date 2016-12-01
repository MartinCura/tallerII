#ifndef PROJECT_CONVERSATION_H
#define PROJECT_CONVERSATION_H

#include "../../jsoncpp/json/json.h"

using namespace std;

class Conversation {

public:
    static const char* WITH_KEY;
    static const char* FIRST_NAME_KEY;
    static const char* LAST_NAME_KEY;
    static const char* UNREAD_COUNT_KEY;

    Conversation();
    ~Conversation();

    void setWith(long with);
    void setFirstName(string firstName);
    void setLastName(string lastName);
    void setUnreadCount(int unreadCount);

    long getWith();
    string getFirstName();
    string getLastName();
    int getUnreadCount();

    Json::Value serializeMe();

private:
    long with;
    string firstName;
    string lastName;
    int unreadCount;
};

#endif //PROJECT_CONVERSATION_H
