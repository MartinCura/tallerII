#include "ContactsHandler.h"
#include "../../Exceptions/NotAuthorizedException.h"
#include "../../Security/Security.h"

ContactsHandler::ContactsHandler() {}

ContactsHandler::~ContactsHandler() {}

Response* ContactsHandler::handleGetRequest(http_message* httpMessage, string url) {
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        vector<Contact*> contacts = personManager->getContactsByUserId(userId);
        Json::Value responseBody;
        responseBody["metadata"]["count"] = contacts.size();
        for (vector<Contact*>::size_type i = 0; i < contacts.size(); i++) {
            Contact* contact = contacts[i];
            Person* person = personManager->getPersonById(contact->getUserId());
            Json::Value contactAsJson;
            contactAsJson["id"] = person->getId();
            contactAsJson["first_name"] = person->getFirstName();
            contactAsJson["last_name"] = person->getLastName();
            WorkHistory *currentJob = person->getCurrentJob();
            if (currentJob != nullptr) {
                contactAsJson["current_job"] = currentJob->serializeMe();
            }
            contactAsJson["status"] = contact->getStatus();
            responseBody["contacts"].append(contactAsJson);
            delete person;
            delete contact;
        }
        response->setSuccessfulHeader();
        response->setBody(responseBody.toStyledString());
    } catch (UserNotFoundException &e) {
        response->setNotFoundHeader();
        response->setErrorBody(e.getMessage(UserNotFoundException::Message::id));
    } catch (InvalidRequestException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    }
    delete personManager;
    return response;
}

Response* ContactsHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* ContactsHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* ContactsHandler::handlePutRequest(http_message* httpMessage, string url) {
    string requestBody = string(httpMessage->body.p);
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    Json::Value parsedBody = this->parseBody(requestBody);
    long author_id = parsedBody["author_id"].asLargestInt();

    //Security
        //Solo puede enviar una solicitud a otro usuario, el dueño de la cuenta.
    if(!Security::hasPermissionToContactUser(this->session->getUserId(), author_id)) throw NotAuthorizedException();
    try {
        personManager->saveOrUpdateRelation(parsedBody);
        response->setSuccessfulHeader();
    } catch (UserNotFoundException &e) {
        response->setNotFoundHeader();
        response->setErrorBody(e.getMessage(UserNotFoundException::Message::id));
    } catch (InvalidRequestException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    }
    delete personManager;
    return response;
}
