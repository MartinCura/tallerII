#include "ContactsHandler.h"

ContactsHandler::ContactsHandler() {}

ContactsHandler::~ContactsHandler() {}

Response* ContactsHandler::handleGetRequest(http_message* httpMessage, string url) {
    long userId = this->getUserIdFromUrl(url);
    //FIXME: path de db a config
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    vector<Person*> contacts = personManager->getPersonFriendsById(userId);
    Json::Value responseBody;
    responseBody["metada"]["count"] = contacts.size();
    for (vector<Person*>::size_type i = 0; i < contacts.size(); i++) {
        Json::Value contactAsJson;
        contactAsJson["id"] = contacts[i]->getId();
        contactAsJson["first_name"] = contacts[i]->getFirstName();
        contactAsJson["last_name"] = contacts[i]->getLastName();
        WorkHistory* currentJob = contacts[i]->getCurrentJob();
        if (currentJob != nullptr) {
            contactAsJson["current_job"] = currentJob->serializeMe();
        }
        //FIXME: soportar los requested status
        contactAsJson["status"] = "active";
        responseBody["contacts"].append(contactAsJson);
    }
    Response* response = new Response();
    response->setSuccessfulHeader();
    response->setBody(responseBody.toStyledString());
    return response;
}

Response* ContactsHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* ContactsHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* ContactsHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}
