#include "AllUsersHandler.h"
#include "../../person/PersonManager.h"

AllUsersHandler::AllUsersHandler() {}

AllUsersHandler::~AllUsersHandler() {}

Response* AllUsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    response->setSuccessfulHeader();
    Json::Value responseBody;
    responseBody["all_users"];
    std::vector<long>* ids = personManager->getAllUsersIds();
    responseBody["metadata"]["count"] = ids->size();
    for (vector<long>::iterator iter = ids->begin() ; iter != ids->end() ; iter++) {
        responseBody["all_users"].append(*iter);
    }
    response->setBody(responseBody.toStyledString());
    delete ids;
    delete personManager;
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
