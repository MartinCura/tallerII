#include "MessagesHandler.h"

const string USER = "user";
const string FROM = "from";
const string TO = "to";

MessagesHandler::MessagesHandler() {}

MessagesHandler::~MessagesHandler() {}

Response* MessagesHandler::handleGetRequest(http_message* httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    try {
        long fromUserId = this->getUserIdFromUrl(url);
        long toUserId = this->getToUserFromQueryParams(queryParams);
        vector<Message*> messages = personManager->getMessages(fromUserId, toUserId);
        messages = this->truncateMessagesIfNeeded(messages, queryParams);
        Json::Value messagesAsJson = this->buildJsonResponse(messages);
        response->setBody(messagesAsJson.toStyledString());
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

Response* MessagesHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* MessagesHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* MessagesHandler::handlePutRequest(http_message* httpMessage, string url) {
    string requestBody = string(httpMessage->body.p);
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    try {
        Json::Value parsedBody = this->parseBody(requestBody);
        personManager->saveMessage(parsedBody);
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

Json::Value MessagesHandler::buildJsonResponse(vector<Message*> messages) {
    Json::Value messagesAsJson;
    messagesAsJson["metadata"]["count"] = messages.size();
    for (vector<long>::size_type i = 0; i < messages.size(); i++) {
        Message* message = messages[i];
        messagesAsJson["messages"].append(message->serializeMe());
        delete message;
    }
    return messagesAsJson;
}

long MessagesHandler::getToUserFromQueryParams(string queryParams) {
    try {
        string toUserIdAsString = this->getParameterFromQueryParams(queryParams, USER);
        long userId = stol(toUserIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}

int MessagesHandler::getFromParameterFromQueryParamsIfExists(string queryParams) {
    size_t found = queryParams.find(FROM);
    if (found == string::npos) {
        return -1;
    }
    try {
        string value = this->getParameterFromQueryParams(queryParams, FROM);
        return stoi(value);
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric parameter");
    }
}

int MessagesHandler::getToParameterFromQueryParamsIfExists(string queryParams) {
    size_t found = queryParams.find(TO);
    if (found == string::npos) {
        return -1;
    }
    try {
        string value = this->getParameterFromQueryParams(queryParams, TO);
        return stoi(value);
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric parameter");
    }
}

vector<Message*> MessagesHandler::truncateMessagesIfNeeded(vector<Message*> messages, string queryParams) {
    if (
        (this->getFromParameterFromQueryParamsIfExists(queryParams) != -1) &&
        (this->getToParameterFromQueryParamsIfExists(queryParams) != -1)
    ) {
        //
    }
    return messages;
}
