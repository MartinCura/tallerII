#include "MessagesHandler.h"
#include "../../Security/Security.h"
#include "../../Exceptions/NotAuthorizedException.h"

const string USER = "user";
const string FIRST = "first";
const string LAST = "last";

MessagesHandler::MessagesHandler() {}

MessagesHandler::~MessagesHandler() {}

Response* MessagesHandler::handleGetRequest(http_message* httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    long fromUserId = this->getUserIdFromUrl(url);
        //Security
            //Solo el auto tiene permiso para leer los mensajes
    if (!Security::hasPermissionToReadMessage(this->session->getUserId(), fromUserId))
      throw NotAuthorizedException();

    try {
        long toUserId = this->getToUserFromQueryParams(queryParams);
        vector<Message*> messages = personManager->getMessages(fromUserId, toUserId);
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
    Json::Value parsedBody = this->parseBody(requestBody);
    long userId = parsedBody["from"].asLargestInt();
      //Seguridad:
    // El usuario solo puede enviar mensaje si es el autor.
    if (!Security::hasPermissionToSendMessage(this->session->getUserId(), userId))
      throw NotAuthorizedException();
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();

    try {

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

int MessagesHandler::getFirstFromQueryParamsIfExists(string queryParams) {
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

int MessagesHandler::getLastFromQueryParamsIfExists(string queryParams) {
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
    int first = this->getFirstFromQueryParamsIfExists(queryParams);
    int last = this->getLastFromQueryParamsIfExists(queryParams);
    if (
        (first != 0) &&
        (last != 0) &&
        (last > first) &&
        (last <= messages.size())
    ) {
        return this->doTruncate(messages, first, last);
    } else if (messages.size() >= 10) {
        first = 1;
        last = 10;
        return this->doTruncate(messages, first, last);
    } else {
        return messages;
    }
}

vector<Message*> MessagesHandler::doTruncate(vector<Message*> messages, int first, int last) {
    unsigned long lastParameter = messages.size() - first + 1;
    unsigned long firstParameter = messages.size() - last + 1;
    vector<Message*> messagesToReturn;
    for (vector<long>::size_type i = 1; i <= firstParameter - 1; i++) {
        Message* message = messages[i - 1];
        delete message;
    }
    for (vector<long>::size_type i = firstParameter; i <= lastParameter; i++) {
        Message* message = messages[i - 1];
        messagesToReturn.push_back(message);
    }
    for (vector<long>::size_type i = lastParameter + 1; i <= messages.size(); i++) {
        Message* message = messages[i - 1];
        delete message;
    }
    return messagesToReturn;
}
