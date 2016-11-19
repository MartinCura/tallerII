#include "ConversationsHandler.h"

ConversationsHandler::ConversationsHandler() {}

ConversationsHandler::~ConversationsHandler() {}

Response* ConversationsHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* ConversationsHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* ConversationsHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* ConversationsHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}
