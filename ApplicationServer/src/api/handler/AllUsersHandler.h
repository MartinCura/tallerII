#ifndef PROJECT_ALLUSERSHANDLER_H
#define PROJECT_ALLUSERSHANDLER_H

#include "Handler.h"

class AllUsersHandler : public Handler {
public:
    AllUsersHandler();
    ~AllUsersHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
};

#endif //PROJECT_ALLUSERSHANDLER_H
