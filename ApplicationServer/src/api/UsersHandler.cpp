#include "UsersHandler.h"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    Response* response = new Response();
    response->setSuccessfulHeader();
    response->setBody("");
    return response;
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}
