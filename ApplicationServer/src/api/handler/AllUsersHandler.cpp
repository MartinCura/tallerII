#include "AllUsersHandler.h"

AllUsersHandler::AllUsersHandler() {}

AllUsersHandler::~AllUsersHandler() {}

Response* AllUsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    Response* response = new Response();
    response->setSuccessfulHeader();
    Json::Value responseBody;
    //FIXME: obtener de la base
    responseBody["all_users"].append(1);
    responseBody["all_users"].append(2);
    response->setBody(responseBody.toStyledString());
    return response;
}

Response* AllUsersHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* AllUsersHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* AllUsersHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}
