#include "AllUsersHandler.h"
#include "../../person/PersonManager.h"

AllUsersHandler::AllUsersHandler() {
    getPublic = true;
}

AllUsersHandler::~AllUsersHandler() {}

Response* AllUsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    response->setSuccessfulHeader();
    Json::Value responseBody;
    std::vector<Person*>* users = personManager->getAllUsers();
    responseBody["metadata"]["count"] = users->size();
    for (int i = 0; i < users->size(); i++) {
        Json::Value user;
        user["first_name"] = (*users)[i]->getFirstName();
        user["last_name"] = (*users)[i]->getLastName();
        user["id"] = (*users)[i]->getId();
        responseBody["all_users"].append(user);
    }
    /*
    std::vector<long>* ids = personManager->getAllUsersIds();
    responseBody["metadata"]["count"] = ids->size();
    for (vector<long>::iterator iter = ids->begin() ; iter != ids->end() ; iter++) {
        responseBody["all_users"].append(*iter);
    }
    response->setBody(responseBody.toStyledString());
    delete ids;*/
    delete users;
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
