#include "RecommendationsHandler.h"

const string TO = "to";

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
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    try {
        long fromUserId = this->getUserIdFromUrl(url);
        long toUserId = this->getToUserFromQueryParams(queryParams);
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
    PersonManager* personManager = new PersonManager("/tmp/appDB/");
    Response* response = new Response();
    try {
        Json::Value parsedBody = this->parseBody(requestBody);
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

