#ifndef PROJECT_NOTIFICATIONTOKENHANDLER_H
#define PROJECT_NOTIFICATIONTOKENHANDLER_H


#include "../../person/PersonManager.h"
#include "../../api/handler/Handler.h"

class NotificationTokenHandler : public Handler{
public:
    NotificationTokenHandler();
    ~NotificationTokenHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
};

#endif //PROJECT_NOTIFICATIONTOKENHANDLER_H
