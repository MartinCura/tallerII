#include "UsersHandler.h"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    string requestBody = string(httpMessage->body.p);
    try {
        Response* response = new Response();
        response->setSuccessfulHeader();
        string responseBody = this->saveOrUpdatePerson(requestBody);
        response->setBody(responseBody);
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    }
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    try {
        Response* response = new Response();
        response->setSuccessfulHeader();
        int userId = this->getUserIdFromUrl(url);
        response->setBody(this->buildGetUserResponse(userId));
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    } catch (UserNotFoundException& e) {
        return this->getNotFoundResponse(e.getMessage(UserNotFoundException::Message::id));
    }
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

string UsersHandler::buildGetUserResponse(int id) {
    PersonManager *personManager = new PersonManager();
    Person *person = personManager->getPersonById(id);
    Json::Value response = person->serializeMe();
    delete person;
    delete personManager;
    return response.toStyledString();
}

string UsersHandler::saveOrUpdatePerson(string body) {

    //TODO: INFORMACIÓN OBLIGATORIA
    Json::Value parsedBody = this->parseBody(body);
    string name = parsedBody.get("name", "").asString();
    if (name == "") {
        throw InvalidRequestException("Missing name parameter.");
    }

    //FIXME: reemplazar por info de la base
    Json::Value root;
    root["id"] = 1;

    return root.toStyledString();
}
