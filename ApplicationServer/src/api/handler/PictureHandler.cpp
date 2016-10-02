#include "PictureHandler.h"

PictureHandler::PictureHandler() {}

PictureHandler::~PictureHandler() {}

Response* PictureHandler::handleGetRequest(http_message* httpMessage, string url) {
    try {
        long userId = this->getUserIdFromUrl(url);
        Response* response = this->buildGetPictureResponse(userId);
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    }
}

Response* PictureHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* PictureHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    long userId = this->getUserIdFromUrl(url);
    Response* response = new Response();
    if(remove(this->getFilePath(userId).c_str()) != 0) {
        response->setNotFoundHeader();
        response->setErrorBody("No such image");
    } else {
        response->setSuccessfulHeader();
    }
    return response;
}

Response* PictureHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* PictureHandler::buildGetPictureResponse(long id) {
    vector<char> buffer;
    FILE* stream = fopen(this->getFilePath(id).c_str(), "rb");
    if (stream == NULL) {
        throw InvalidRequestException("Cannot find requested picture");
    }
    fseek(stream, 0, SEEK_END);
    unsigned long length = (unsigned long) ftell(stream);
    rewind(stream);
    buffer.resize(length);
    int lengthRead = fread(&buffer[0], 1, length, stream);
    fclose(stream);
    if (lengthRead == 0) {
        throw InvalidRequestException("Failed to open requested picture");
    }
    Response* response = new Response();
    response->setSuccessfulHeader();
    response->setBinaryBody(&buffer[0], length, "image/jpeg");
    return response;
}

string PictureHandler::getFilePath(long id) {
    return "../ApplicationServer/img/profile/" + to_string(id) + ".jpg";
}
