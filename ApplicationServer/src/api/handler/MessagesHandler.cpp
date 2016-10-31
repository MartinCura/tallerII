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
        int totalCount = messages.size();
        messages = this->truncateMessages(messages, queryParams);
        Json::Value messagesAsJson = this->buildJsonResponse(messages, totalCount);
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

Json::Value MessagesHandler::buildJsonResponse(vector<Message*> messages, int totalCount) {
    Json::Value messagesAsJson;
    messagesAsJson["metadata"]["count"] = messages.size();
    messagesAsJson["metadata"]["total_count"] = totalCount;
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
        return 0;
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
        return 0;
    }
    try {
        string value = this->getParameterFromQueryParams(queryParams, TO);
        return stoi(value);
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric parameter");
    }
}

vector<Message*> MessagesHandler::truncateMessages(vector<Message*> messages, string queryParams) {
    int fromParameter = this->getFromParameterFromQueryParamsIfExists(queryParams);
    int toParameter = this->getToParameterFromQueryParamsIfExists(queryParams);
    if (
        (fromParameter != 0) &&
        (toParameter != 0) &&
        (toParameter > fromParameter) &&
        (toParameter <= messages.size())
    ) {
        return this->doTruncate(messages, fromParameter, toParameter);
    } else if (messages.size() >= 10) {
        int fromParameter = 1;
        int toParameter = 10;
        return this->doTruncate(messages, fromParameter, toParameter);
    } else {
        return messages;
    }
}

vector<Message*> MessagesHandler::doTruncate(vector<Message*> messages, int from, int to) {
    int toParameter = messages.size() - from + 1;
    int fromParameter = messages.size() - to + 1;
    vector<Message*> messagesToReturn;
    for (vector<long>::size_type i = 1; i <= fromParameter - 1; i++) {
        Message* message = messages[i - 1];
        delete message;
    }
    for (vector<long>::size_type i = fromParameter; i <= toParameter; i++) {
        Message* message = messages[i - 1];
        messagesToReturn.push_back(message);
    }
    for (vector<long>::size_type i = toParameter + 1; i <= messages.size(); i++) {
        Message* message = messages[i - 1];
        delete message;
    }
    return messagesToReturn;
}
