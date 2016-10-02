#include "AllUsersHandler.h"
#include "../../person/PersonManager.h"

AllUsersHandler::AllUsersHandler() {}

AllUsersHandler::~AllUsersHandler() {}

Response* AllUsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    Response* response = new Response();
    response->setSuccessfulHeader();
    Json::Value responseBody;
    //FIXME: path de db a config
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    std::vector<string>* ids = personManager->getAllUsersIds();
    for (vector<string>::iterator iter = ids->begin() ; iter != ids->end() ; iter++) {
        responseBody["all_users"].append(*iter);
    }

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
