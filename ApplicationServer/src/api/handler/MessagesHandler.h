#ifndef PROJECT_MESSAGESHANDLER_H
#define PROJECT_MESSAGESHANDLER_H

#include "../../person/PersonManager.h"
#include "Handler.h"
#include "../../person/messages/Message.h"

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
    int getFromParameterFromQueryParamsIfExists(string queryParams);
    int getToParameterFromQueryParamsIfExists(string queryParams);
    vector<Message*> truncateMessagesIfNeeded(vector<Message*> messages, string queryParams);
    Json::Value buildJsonResponse(vector<Message*> messages);
};

#endif //PROJECT_MESSAGESHANDLER_H
