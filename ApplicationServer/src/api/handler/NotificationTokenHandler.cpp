#include "NotificationTokenHandler.h"

NotificationTokenHandler::NotificationTokenHandler() {}

NotificationTokenHandler::~NotificationTokenHandler() {}

Response* NotificationTokenHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* NotificationTokenHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* NotificationTokenHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* NotificationTokenHandler::handlePutRequest(http_message* httpMessage, string url) {
    string requestBody = string(httpMessage->body.p);
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        if (!Security::hasPermissionToUpdateNotificationToken(this->session->getUserId(), userId)) {
            delete personManager;
            delete response;
            throw NotAuthorizedException();
        }
        Json::Value parsedBody = this->parseBody(requestBody);
        personManager->setOrUpdateNotificationToken(parsedBody, userId);
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
