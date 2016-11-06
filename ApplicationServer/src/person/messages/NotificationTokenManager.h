#ifndef PROJECT_NOTIFICATIONTOKENMANAGER_H
#define PROJECT_NOTIFICATIONTOKENMANAGER_H

#include "../../DB/DBWrapper.h"
#include "../../jsoncpp/json/json.h"
#include "../../Exceptions/NonexistentNotificationToken.h"
#include <iostream>
#include <string>

using namespace std;

class NotificationTokenManager {

public:
    static const string TOKEN_KEY;

    NotificationTokenManager(DBWrapper* db);
    ~NotificationTokenManager();

    string getTokenByUserId(long userId);
    void setOrUpdateToken(long userId, string token);

private:
    DBWrapper* db;
};

#endif //PROJECT_NOTIFICATIONTOKENMANAGER_H
