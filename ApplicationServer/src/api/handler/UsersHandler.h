#ifndef APPLICATIONSERVER_USERSHANDLER_H
#define APPLICATIONSERVER_USERSHANDLER_H

#include "Handler.h"
#include "../../person/PersonManager.h"
#include "../../person/Person.h"
#include "../../person/WorkHistory.h"
#include "../exception/UserNotFoundException.h"
#include "../exception/InvalidRequestException.h"
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
    string buildGetUserResponse(int id);
    string saveOrUpdatePerson(string body);
};

#endif //APPLICATIONSERVER_USERSHANDLER_H
