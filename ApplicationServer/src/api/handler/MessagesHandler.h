#ifndef PROJECT_MESSAGESHANDLER_H
#define PROJECT_MESSAGESHANDLER_H

#include "../../person/PersonManager.h"
#include "Handler.h"
#include "../../person/messages/Message.h"
#include "../../logger/Logger.h"
#include "../../Exceptions/NonexistentNotificationToken.h"
#include <algorithm>
#include <vector>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class MessagesHandler : public Handler {
public:
    MessagesHandler();
    ~MessagesHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);

private:
    long getToUserFromQueryParams(string queryParams);
    int getFirstFromQueryParamsIfExists(string queryParams);
    int getLastFromQueryParamsIfExists(string queryParams);
    vector<Message*> truncateMessages(vector<Message*> messages, string queryParams);
    vector<Message*> doTruncate(vector<Message*> messages, int first, int last);
    Json::Value buildJsonResponse(vector<Message*> messages, long totalCount);
    void sendNotification(string savedMessage, PersonManager* personManager);
    string buildRequest(string savedMessage, PersonManager* personManager);
    string buildStringRequest(long fromUserId, long toUserId, string message, string timestamp, string token);
    string performRequest(string request);
};

#endif //PROJECT_MESSAGESHANDLER_H
