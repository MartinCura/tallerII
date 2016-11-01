#include "MessagesHandler.h"

const string USER = "user";
const string FIRST = "first";
const string LAST = "last";

MessagesHandler::MessagesHandler() {}

MessagesHandler::~MessagesHandler() {}

Response* MessagesHandler::handleGetRequest(http_message* httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    try {
        long fromUserId = this->getUserIdFromUrl(url);
        long toUserId = this->getToUserFromQueryParams(queryParams);
        vector<Message*> messages = this->getAllMessages(fromUserId, toUserId, personManager);
        long totalCount = messages.size();
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

Json::Value MessagesHandler::buildJsonResponse(vector<Message*> messages, long totalCount) {
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
    size_t found = queryParams.find(FIRST);
    if (found == string::npos) {
        return 0;
    }
    try {
        string value = this->getParameterFromQueryParams(queryParams, FIRST);
        return stoi(value);
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric parameter");
    }
}

int MessagesHandler::getToParameterFromQueryParamsIfExists(string queryParams) {
    size_t found = queryParams.find(LAST);
    if (found == string::npos) {
        return 0;
    }
    try {
        string value = this->getParameterFromQueryParams(queryParams, LAST);
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
        fromParameter = 1;
        toParameter = 10;
        return this->doTruncate(messages, fromParameter, toParameter);
    } else {
        return messages;
    }
}

bool sortByTimestamp(Message* message1, Message* message2) {
    time_t timestamp1 = message1->getTimestampAsTimeT();
    time_t timestamp2 = message2->getTimestampAsTimeT();
    double seconds = difftime(timestamp1, timestamp2);
    return (seconds < 0.0);
}

vector<Message*> MessagesHandler::getAllMessages(long userId1, long userId2, PersonManager* personManager) {
    vector<Message*> messages1 = personManager->getMessages(userId1, userId2);
    vector<Message*> messages2 = personManager->getMessages(userId2, userId1);
    messages1.insert(messages1.end(), messages2.begin(), messages2.end());
    //sort(messages1.begin(), messages1.end(), sortByTimestamp);
    return messages1;
}

vector<Message*> MessagesHandler::doTruncate(vector<Message*> messages, int from, int to) {
    unsigned long toParameter = messages.size() - from + 1;
    unsigned long fromParameter = messages.size() - to + 1;
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
