#include "UsersHandler.h"

#define NAME_DB "/tmp/appDB/"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
    /*string requestBody = string(httpMessage->body.p);
    PersonManager *personManager = new PersonManager(NAME_DB);
    Response* response = new Response();
    try {
        Json::Value responseBody;
        Json::Value parsedBody = this->parseBody(requestBody);
        responseBody["id"] = personManager->savePerson(parsedBody);
        response->setSuccessfulHeader();
        response->setBody(responseBody.toStyledString());
    } catch (UserAlreadyExistsException& e) {
        response->setConflictHeader();
        response->setErrorBody(e.what());
    } catch (InvalidRequestException& e) {
        response = this->getBadRequestResponse(e.getMessage());
    }
    delete personManager;
    return response;*/
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    PersonManager *personManager = new PersonManager(NAME_DB);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        Person *person = personManager->getPersonById(userId);
        response->setSuccessfulHeader();
        response->setBody(person->serializeMe().toStyledString());
        delete person;
    } catch (InvalidRequestException& e) {
        response = this->getBadRequestResponse(e.getMessage());
    } catch (UserNotFoundException& e) {
        response = this->getNotFoundResponse(e.getMessage(UserNotFoundException::Message::id));
    }
    delete personManager;
    return response;
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    PersonManager *personManager = new PersonManager(NAME_DB);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        personManager->deletePerson(userId);
        response->setSuccessfulHeader();
    } catch (InvalidRequestException& e) {
        response = this->getBadRequestResponse(e.getMessage());
    } catch (UserNotFoundException& e) {
        response = this->getNotFoundResponse(e.getMessage(UserNotFoundException::Message::id));
    }
    delete personManager;
    return response;
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}
