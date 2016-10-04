//
// Created by milena on 02/10/16.
//

#include "LoginHandler.h"
#include "../../person/PersonManager.h"
#include "../../session/SessionManager.h"
#include "../../Exceptions/InvalidPasswordException.h"

#define NAME_DB "/tmp/appDB/"

Response *LoginHandler::handlePostRequest(http_message *httpMessage) {
    string user_password, user_mail;
    Json::Value body;
    PersonManager* personManager;
    SessionManager* sessionManager;
    std::string token;

    try {
        body = this->parseBody(string(httpMessage->body.p));

        //Validations

        if (!body.isMember("mail")) {
            throw InvalidRequestException("Mail member is missing");
        }
        if (!body.isMember("password")) {
            throw InvalidRequestException("Password member is missing");
        }
    } catch (InvalidRequestException &e) {
        return this->getBadRequestResponse(e.getMessage());
    }

    user_mail = body["mail"].asString();
    user_password = body["password"].asString();

    try {
        personManager = new PersonManager(NAME_DB);
        personManager->login(user_mail, user_password);

        //Loggin aceptado
        delete personManager;
        sessionManager = new SessionManager(NAME_DB); //TODO: VERIFICAR MULTIPLES CONEXIONES DE BD
        token = sessionManager->getNewToken(user_mail);
        Json::Value responseBody; //TODO: METERLO EN COOKIE
        responseBody["token"] = token;
        Response* response = new Response();
        response->setSuccessfulHeader();
        response->setBody(responseBody.toStyledString());

        return response;

    } catch (std::exception& e) {
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

