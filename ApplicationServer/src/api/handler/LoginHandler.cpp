//
// Created by milena on 02/10/16.
//

#include "LoginHandler.h"
#include "../../person/PersonManager.h"

#define NAME_DB "/tmp/appDB/"

Response *LoginHandler::handlePostRequest(http_message *httpMessage) {
    string user_password, user_mail;
    Json::Value body;
    PersonManager* personManager;

    body = this->parseBody(string(httpMessage->body.p));

    //Validations

    //TODO: SEE VALIDATIONS

    if (!body.isMember("mail")) {
        throw InvalidRequestException("Mail member is missing");
    }
    if (!body.isMember("password")) {
        throw InvalidRequestException("Password member is missing");
    }

    user_mail = body["mail"].asString();
    user_password = body["password"].asString();

    try {
        personManager = new PersonManager(NAME_DB);
        personManager->login(user_mail, user_password);

        delete personManager;
        //TODO: MISSING RESPONSE
        return new Response();

    } catch (UserNotFoundException& e) {
        Response* response = new Response();
        response->setConflictHeader();
        response->setErrorBody(e.what());
        return response;
    }
}

Response* LoginHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* LoginHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* LoginHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

