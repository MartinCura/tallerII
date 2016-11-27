#include "MessagesHandler.h"
#include "../../Security/Security.h"
#include "../../Exceptions/NotAuthorizedException.h"

const string USER = "user";
const string FIRST = "first";
const string LAST = "last";
const int FIRST_INDEX = 1;
const int MAX_CANT = 10;

MessagesHandler::MessagesHandler() {}

MessagesHandler::~MessagesHandler() {}

Response* MessagesHandler::handleGetRequest(http_message* httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long fromUserId = this->getUserIdFromUrl(url);
        //Security
        //Solo el auto tiene permiso para leer los mensajes
        if (!Security::hasPermissionToReadMessage(this->session->getUserId(), fromUserId)) throw NotAuthorizedException();
        long toUserId = this->getToUserFromQueryParams(queryParams);
        vector<Message*> messages = personManager->getMessages(fromUserId, toUserId);
        long totalCount = messages.size();
        messages = this->truncateMessages(messages, queryParams);
        personManager->setMessagesAsReceived(fromUserId, toUserId, messages);
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
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        Json::Value parsedBody = this->parseBody(requestBody);
        if (!parsedBody.isMember("from")) throw InvalidRequestException("Missing from user");
        if (!parsedBody.isMember("to")) throw InvalidRequestException("Missing to user");
        if (!parsedBody.isMember("message")) throw InvalidRequestException("Missing message");
        long userId = parsedBody["from"].asLargestInt();
        //Seguridad:
        // El usuario solo puede enviar mensaje si es el autor.
        if (!Security::hasPermissionToSendMessage(this->session->getUserId(), userId)) throw NotAuthorizedException();
        string savedMessage = personManager->saveMessage(parsedBody);
        this->sendNotification(savedMessage, personManager);
        response->setSuccessfulHeader();
        response->setBody(savedMessage);
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
    int firstParam = this->getFirstFromQueryParamsIfExists(queryParams);
    int lastParam = this->getLastFromQueryParamsIfExists(queryParams);
    if ((firstParam < 0) || (lastParam < 0)) {
        throw InvalidRequestException("index must be greater than 0");
    }
    int first = (firstParam != 0) ? firstParam : FIRST_INDEX;
    int last = (lastParam != 0) ? lastParam : (firstParam + MAX_CANT);
    if (last < first) {
        throw InvalidRequestException("first can not be greater than last");
    }
    if (last > messages.size()) {
        last = (int) messages.size();
    }
    return this->doTruncate(messages, first, last);
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

void MessagesHandler::sendNotification(string savedMessage, PersonManager* personManager) {
    try {
        Json::Value root;
        Json::Reader reader;
        reader.parse(savedMessage, root);
        long fromUserId = root["from"].asLargestInt();
        long toUserId = root["to"].asLargestInt();
        string message = root["message"].asString();
        string timestamp = root["timestamp"].asString();

        string token = personManager->getNotificationTokenByUserId(toUserId);

        NotificationSender* notificationSender = new NotificationSender();
        string request = notificationSender->buildMessageRequest(fromUserId, toUserId, message, timestamp, token);
        notificationSender->sendNotification(request);
        delete notificationSender;
    } catch (NonexistentNotificationToken &e) {}
}
