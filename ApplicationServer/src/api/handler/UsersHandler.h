#ifndef APPLICATIONSERVER_USERSHANDLER_H
#define APPLICATIONSERVER_USERSHANDLER_H

#include "Handler.h"
#include "../../person/PersonManager.h"
#include "../../person/Person.h"
#include "../../person/WorkHistory.h"
#include "../../Exceptions/UserNotFoundException.h"
#include "../../Exceptions/InvalidRequestException.h"
#include "../../Exceptions/UserAlreadyExistsException.h"
#include <vector>

class UsersHandler : public Handler {
public:
    bool isPublic = false;

    UsersHandler();
    ~UsersHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);

};

#endif //APPLICATIONSERVER_USERSHANDLER_H
