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
    return this->getNotImplementedResponse();
}
