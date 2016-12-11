#include "ContactsHandler.h"
#include "../../Exceptions/NotAuthorizedException.h"
#include "../../Security/Security.h"

ContactsHandler::ContactsHandler() {}

ContactsHandler::~ContactsHandler() {}

Response* ContactsHandler::handleGetRequest(http_message* httpMessage, string url) {
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        vector<Contact*> contacts = personManager->getContactsByUserId(userId);
        Json::Value responseBody;
        responseBody["metadata"]["count"] = contacts.size();
        for (vector<Contact*>::size_type i = 0; i < contacts.size(); i++) {
            Contact* contact = contacts[i];
            Person* person = personManager->getUserById(contact->getUserId());
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
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
   try {
       Json::Value parsedBody = this->parseBody(requestBody);
       if (!parsedBody.isMember("action")) throw InvalidRequestException("Missing action");
       if (!parsedBody.isMember("author_id")) throw InvalidRequestException("Missing author_id");
       if (!parsedBody.isMember("contact_id")) throw InvalidRequestException("Missing contact_id");
       long author_id = parsedBody["author_id"].asLargestInt();
       //Security
       //Solo puede enviar una solicitud a otro usuario, el dueño de la cuenta.
       if(!Security::hasPermissionToContactUser(this->session->getUserId(), author_id)) {
           delete personManager;
           delete response;
           throw NotAuthorizedException();
       }
       personManager->saveOrUpdateRelation(parsedBody);
       if (parsedBody["action"] == RelationsManager::ADD_CONTACT_ACTION) {
           this->sendNotification(parsedBody, personManager);
       }
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

void ContactsHandler::sendNotification(Json::Value parsedBody, PersonManager* personManager) {
    try {
        long fromUserId = parsedBody["author_id"].asLargestInt();
        long toUserId = parsedBody["contact_id"].asLargestInt();

        string token = personManager->getNotificationTokenByUserId(toUserId);

        Person* person = personManager->getUserById(fromUserId);
        string name = person->getFirstName() + " " + person->getLastName();
        delete person;

        NotificationSender* notificationSender = new NotificationSender();
        string request = notificationSender->buildContactRequest(fromUserId, toUserId, name, token);
        notificationSender->sendNotification(request);
        delete notificationSender;
    } catch (NonexistentNotificationToken &e) {}
}
