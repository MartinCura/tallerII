#ifndef PROJECT_NOTIFICATIONSENDER_H
#define PROJECT_NOTIFICATIONSENDER_H

#include <algorithm>
#include <vector>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "../logger/Logger.h"

using namespace std;

class NotificationSender {
public:
    NotificationSender();
    ~NotificationSender();
    void sendNotification(string request);
    string buildMessageRequest(long fromUserId, long toUserId, string message, string timestamp, string token);

private:
    string buildCommonRequest(string data, string token);
    string performRequest(string request);
};

#endif //PROJECT_NOTIFICATIONSENDER_H
