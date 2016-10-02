#include "UsersHandler.h"

#define NAME_DB "/tmp/appDB/"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    string requestBody = string(httpMessage->body.p);
    try {
        PersonManager *personManager = new PersonManager(NAME_DB);
        Json::Value responseBody;
        responseBody["id"] = personManager->savePerson(this->parseBody(requestBody));
        Response* response = new Response();
        response->setSuccessfulHeader();
        response->setBody(responseBody.toStyledString());
        delete personManager;
        return response;
    } catch (UserAlreadyExistsException& e) {
        Response* response = new Response();
        response->setConflictHeader();
        response->setErrorBody(e.what());
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    }
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    try {
        PersonManager *personManager = new PersonManager(NAME_DB);
        Person *person = personManager->getPersonById(this->getUserIdFromUrl(url));
        Response* response = new Response();
        response->setSuccessfulHeader();
        response->setBody(person->serializeMe().toStyledString());
        delete person;
        delete personManager;
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    } catch (UserNotFoundException& e) {
        return this->getNotFoundResponse(e.getMessage(UserNotFoundException::Message::id));
    }
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}
