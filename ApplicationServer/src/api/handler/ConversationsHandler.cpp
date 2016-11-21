#include "ConversationsHandler.h"

ConversationsHandler::ConversationsHandler() {}

ConversationsHandler::~ConversationsHandler() {}

Response* ConversationsHandler::handleGetRequest(http_message* httpMessage, string url) {
    //TODO: AGREGAR VALIDACION DE SEGURIDAD

    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        vector<Conversation*> conversations = personManager->getAllConversations(userId);
        long totalCount = conversations.size();
        Json::Value conversationsAsJson = this->buildJsonResponse(conversations, totalCount);
        response->setBody(conversationsAsJson.toStyledString());
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

Response* ConversationsHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* ConversationsHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* ConversationsHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Json::Value ConversationsHandler::buildJsonResponse(vector<Conversation*> conversations, long totalCount) {
    Json::Value conversationsAsJson;
    int totalUnreadCount = 0;
    conversationsAsJson["metadata"]["total_count"] = totalCount;
    for (vector<long>::size_type i = 0; i < conversations.size(); i++) {
        Conversation* conversation = conversations[i];
        totalUnreadCount += conversation->getUnreadCount();
        conversationsAsJson["conversations"].append(conversation->serializeMe());
        delete conversation;
    }
    conversationsAsJson["metadata"]["total_unread_count"] = totalUnreadCount;
    return conversationsAsJson;
}
