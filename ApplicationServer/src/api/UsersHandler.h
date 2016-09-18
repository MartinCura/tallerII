#ifndef APPLICATIONSERVER_USERSHANDLER_H
#define APPLICATIONSERVER_USERSHANDLER_H

#include "Handler.h"
#include "../person/PersonManager.h"
#include "../person/Person.h"
#include "../person/WorkHistory.h"
#include "exception/UserNotFoundException.h"
#include "exception/InvalidRequestException.h"
#include <vector>

class UsersHandler : public Handler {
public:
    UsersHandler();
    ~UsersHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage);
    Response* handlePutRequest(http_message* httpMessage);

private:
    int getUserId(string url);
    const char* buildResponse(int id);
};

#endif //APPLICATIONSERVER_USERSHANDLER_H
