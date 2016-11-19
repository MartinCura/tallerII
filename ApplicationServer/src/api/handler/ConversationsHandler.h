#ifndef PROJECT_CONVERSATIONSHANDLER_H
#define PROJECT_CONVERSATIONSHANDLER_H

#include "../../api/handler/Handler.h"

class ConversationsHandler : public Handler {
public:
    ConversationsHandler();
    ~ConversationsHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
};

#endif //PROJECT_CONVERSATIONSHANDLER_H
