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
    SessionManager* sessionManager = nullptr;
    std::string token;

    try {
        body = this->parseBody(string(httpMessage->body.p));

        //Validations

        if (!body.isMember("email")) {
            throw InvalidRequestException("Mail member is missing");
        }
        if (!body.isMember("password")) {
            throw InvalidRequestException("Password member is missing");
        }
    } catch (InvalidRequestException &e) {
        Response* response = new Response();
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
        return response;
    }

    user_mail = body["email"].asString();
    user_password = body["password"].asString();

    try {
        personManager = new PersonManager(NAME_DB);
        personManager->login(user_mail, user_password);

    } catch (InvalidPasswordException& e) {

        Response *response = new Response();
        response->setConflictHeader();
        response->setErrorBody(e.what());
        delete personManager;
        return response;
    } catch (UserNotFoundException& e) {
        Response *response = new Response();
        response->setConflictHeader();
        response->setErrorBody(e.getMessage(UserNotFoundException::Message::mail));
        //TODO no tiene sentido que repita logica porque los mensajes se manejan distintos para este tipo de excepción, MEjorar.
        delete personManager;
        return response;
    }

    //Loggin aceptado
    delete personManager;

    sessionManager = new SessionManager(NAME_DB); //TODO: VERIFICAR MULTIPLES CONEXIONES DE BD
    token = sessionManager->getNewToken(user_mail);
    Json::Value responseBody; //TODO: METERLO EN COOKIE
    responseBody["token"] = token;
    Response* response = new Response();
    response->setSuccessfulHeader();
    response->setBody(responseBody.toStyledString());

    delete sessionManager;
    return response;

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

