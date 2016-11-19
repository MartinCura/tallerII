#ifndef PROJECT_LOGINWITHFACEBOOKHANDLER_H
#define PROJECT_LOGINWITHFACEBOOKHANDLER_H

#include "../../api/handler/Handler.h"
#include "../../api/handler/LoginHandler.h"
#include "../../person/PersonManager.h"
#include "../../person/Person.h"
#include "../../logger/Logger.h"
#include "../../Exceptions/ErrorLoginWithFacebookException.h"
#include "../../session/SessionManager.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <regex>

using namespace std;

class LoginWithFacebookHandler : public Handler {
public:
    static const string TOKEN_KEY;
    static const string FIRST_NAME;
    static const string LAST_NAME;
    static const string BIRTHDAY;
    static const string EMAIL;
    LoginWithFacebookHandler();
    ~LoginWithFacebookHandler();

    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);

private:
    Json::Value getUserDataFromFacebook(Json::Value request);
    string buildStringRequest(string tokenToSend);
    string performRequest(string request);
    Json::Value parseResponse(string response);
    Person* createPerson(Json::Value data);
};

#endif //PROJECT_LOGINWITHFACEBOOKHANDLER_H
