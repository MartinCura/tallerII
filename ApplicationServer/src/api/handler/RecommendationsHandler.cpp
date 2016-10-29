#include "RecommendationsHandler.h"

RecommendationsHandler::RecommendationsHandler() {}

RecommendationsHandler::~RecommendationsHandler() {}

Response* RecommendationsHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
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

Response* RecommendationsHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* RecommendationsHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* RecommendationsHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

