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
        root["id"] = "1";

        string responseBody = root.toStyledString();
        response->setBody(responseBody.c_str());

        return response;
    } catch (const char* e) {
        return this->getBadRequestResponse(e);
    }
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    try {
        Response* response = new Response();
        response->setSuccessfulHeader();

        //FIXME: reemplazar por info de la base
        string name = "John";
        string id = this->getUserId(url);
        Json::Value root;
        root["id"] = id;
        root["name"] = name;

        string responseBody = root.toStyledString();
        response->setBody(responseBody.c_str());
        return response;
    } catch (const char* e) {
        return this->getBadRequestResponse(e);
    }
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

string UsersHandler::getUserId(string url) {
    size_t sp = url.find_first_of('/', 1);
    if (sp == string::npos || ((url.begin() + sp + 1) >= (url.begin() + url.size()))) {
        throw "Cannot get user id from url.";
    }
    string userId(url.begin() + sp + 1, url.begin() + url.size());
    return userId;
}
