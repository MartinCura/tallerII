#include "UsersHandler.h"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    string body = string(httpMessage->body.p);
    try {
        Json::Value parsedBody = this->parseBody(body);
        string name = parsedBody.get("name", "").asString();
        if (name == "") {
            return this->getBadRequestResponse("Missing name parameter.");
        }
        Response* response = new Response();
        response->setSuccessfulHeader();

        //FIXME: reemplazar por info de la base
        Json::Value root;
        root["id"] = 1;

        string responseBody = root.toStyledString();
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
        response->setBody(this->buildGetUserResponse(this->getUserId(url)));
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    } catch (UserNotFoundException& e) {
        return this->getNotFoundResponse(e.getMessage());
    }
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

int UsersHandler::getUserId(string url) {
    size_t sp = url.find_first_of('/', 1);
    if (sp == string::npos || ((url.begin() + sp + 1) >= (url.begin() + url.size()))) {
        throw InvalidRequestException("Cannot get user id from url.");
    }
    string userIdAsString(url.begin() + sp + 1, url.begin() + url.size());
    try {
        int userId = stoi(userIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}

string UsersHandler::buildGetUserResponse(int id) {
    PersonManager *personManager = new PersonManager();
    Person *person = personManager->getPersonById(id);
    Json::Value response = person->serializeMe();
    delete person;
    delete personManager;
    return response.toStyledString();
}
