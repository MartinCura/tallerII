//
// Created by milena on 02/10/16.
//

#ifndef APPLICATIONSERVER_LOGINHANDLER_H
#define APPLICATIONSERVER_LOGINHANDLER_H


#include "Handler.h"

class LoginHandler : public Handler{
public:

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);

private:

};


#endif //APPLICATIONSERVER_LOGINHANDLER_H
