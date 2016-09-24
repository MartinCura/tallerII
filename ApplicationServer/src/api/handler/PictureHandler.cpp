#include "PictureHandler.h"

PictureHandler::PictureHandler() {}

PictureHandler::~PictureHandler() {}

Response* PictureHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* PictureHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* PictureHandler::handleDeleteRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* PictureHandler::handlePutRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}
