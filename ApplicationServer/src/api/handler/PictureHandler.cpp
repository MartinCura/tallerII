#include "PictureHandler.h"

PictureHandler::PictureHandler() {}

PictureHandler::~PictureHandler() {}

Response* PictureHandler::handleGetRequest(http_message* httpMessage, string url) {
    try {
        int userId = this->getUserIdFromUrl(url);
        Response* response = this->buildGetPictureResponse(userId);
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    }
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

Response* PictureHandler::buildGetPictureResponse(int id) {
    string path = "../ApplicationServer/img/profile/" + to_string(id) + ".jpg";
    vector<char> buffer;
    FILE* stream = fopen(path.c_str(), "rb");
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
    response->setSuccessfulBody();
    return response;
}
