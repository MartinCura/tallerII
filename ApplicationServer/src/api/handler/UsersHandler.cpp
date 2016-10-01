#include "UsersHandler.h"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    string requestBody = string(httpMessage->body.p);
    try {
        Response* response = new Response();
        PersonManager *personManager = new PersonManager();
        response->setSuccessfulHeader();
        Json::Value responseBody;
        responseBody["id"] = personManager->savePerson(this->parseBody(requestBody));
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
        Response* response = new Response();
        PersonManager *personManager = new PersonManager();
        Person *person = personManager->getPersonById(this->getUserIdFromUrl(url));
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
