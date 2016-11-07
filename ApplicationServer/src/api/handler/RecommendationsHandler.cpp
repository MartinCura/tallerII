#include "RecommendationsHandler.h"
#include "../../Security/Security.h"
#include "../../Exceptions/NotAuthorizedException.h"

const string TO = "to";
const string FROM = "from";

RecommendationsHandler::RecommendationsHandler() {}

RecommendationsHandler::~RecommendationsHandler() {}

Response* RecommendationsHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* RecommendationsHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* RecommendationsHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long fromUserId = this->getFromUserFromQueryParams(queryParams);
        long toUserId = this->getToUserFromQueryParams(queryParams);
        //Seguridad:
        // El usuario solo puede enviar recomendaciones  si es el autor.
        if (!Security::hasPermissionToDeleteRecommendations(this->session->getUserId(), fromUserId))throw NotAuthorizedException();
        personManager->removeRecommendation(fromUserId, toUserId);
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

Response* RecommendationsHandler::handlePutRequest(http_message* httpMessage, string url) {
    string requestBody = string(httpMessage->body.p);
    PersonManager* personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        Json::Value parsedBody = this->parseBody(requestBody);
        if (!parsedBody.isMember("from")) throw InvalidRequestException("Missing from user");
        if (!parsedBody.isMember("to")) throw InvalidRequestException("Missing to user");
        long userId = parsedBody["from"].asLargestInt();
        //Seguridad:
        // El usuario solo puede enviar recomendaciones  si es el autor.
        if (!Security::hasPermissionToSendRecommendations(this->session->getUserId(), userId))throw NotAuthorizedException();
        personManager->saveRecommendation(parsedBody);
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

long RecommendationsHandler::getToUserFromQueryParams(string queryParams) {
    try {
        string toUserIdAsString = this->getParameterFromQueryParams(queryParams, TO);
        long userId = stol(toUserIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}


long RecommendationsHandler::getFromUserFromQueryParams(string queryParams) {
    try {
        string toUserIdAsString = this->getParameterFromQueryParams(queryParams, FROM);
        long userId = stol(toUserIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}
