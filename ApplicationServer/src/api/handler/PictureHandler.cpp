#include "PictureHandler.h"
#include "../../Security/Security.h"
#include "../../Exceptions/NotAuthorizedException.h"

PictureHandler::PictureHandler() {
    getPublic = true;
}

PictureHandler::~PictureHandler() {}

Response* PictureHandler::handleGetRequest(http_message* httpMessage, string url) {
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        response = this->buildGetPictureResponse(userId);
    } catch (InvalidRequestException& e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    } catch (PictureNotFoundException& e) {
        response->setNotFoundHeader();
        response->setErrorBody(e.getMessage());
    }
    return response;
}

Response* PictureHandler::handlePostRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* PictureHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    long userId = this->getUserIdFromUrl(url);

    //Security
        //Solo puede borrar la imagen el usuario "dueño" de la misma.
    if (!Security::hasPermissionToDeleteProfilePicture(this->session->getUserId(), userId)) throw NotAuthorizedException();
    Response* response = new Response();
    if(!this->existsPictureForId(userId)) {
        response->setNotFoundHeader();
        response->setErrorBody("No such image");
    } else {
        try {
            this->deletePicture(userId);
            response->setSuccessfulHeader();
        } catch (ErrorDeletingFileException &e) {
            response->setInternalServerErrorHeader();
            response->setErrorBody(e.getMessage());
        }
    }
    return response;
}

Response* PictureHandler::handlePutRequest(http_message* httpMessage, string url) {
    long userId = this->getUserIdFromUrl(url);
    //Security
    //Solo puede editar la imagen el usuario "dueño" de la misma.
    if (!Security::hasPermissionToEditProfilePicture(this->session->getUserId(), userId)) throw NotAuthorizedException();
    Response* response = new Response;
    if (this->existsPictureForId(userId)) {
        try {
            this->deletePicture(userId);
        } catch (ErrorDeletingFileException &e) {
            response->setInternalServerErrorHeader();
            response->setErrorBody(e.getMessage());
            return response;
        }
    }
    this->savePicture(userId, httpMessage->body.p, httpMessage->body.len);
    response->setSuccessfulHeader();
    return response;
}

Response* PictureHandler::buildGetPictureResponse(long id) {
    vector<char> buffer;
    FILE* stream = fopen(this->getFilePath(id).c_str(), "rb");
    if (stream == NULL) {
        throw PictureNotFoundException("Cannot find requested picture");
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
    return Config::getInstance()->get(Config::IMG_FOLDER) + to_string(id) + ".jpeg";
}

bool PictureHandler::existsPictureForId(long id) {
    string path = this->getFilePath(id);
    if (FILE *file = fopen(path.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

void PictureHandler::deletePicture(long id) {
    string path = this->getFilePath(id);
    if(remove(path.c_str()) != 0) {
        throw ErrorDeletingFileException("An error has occurred trying to delete file " + path);
    }
}

void PictureHandler::savePicture(long id, const char* body, size_t size) {
    string path = this->getFilePath(id);
    const char* lineBreak= "\n";
    int lineBreaksCounter = 0;
    int charactersCounter = 0;
    for (int i = 0; i < 200; i++) {
        if (lineBreaksCounter == 4) break;
        if (*body == *lineBreak) {
            lineBreaksCounter++;
        }
        body++;
        charactersCounter++;
    }
    ofstream outfile;
    outfile.open(path, ios::binary | ios::out);
    outfile.write(body, (size - charactersCounter - 4));
    outfile.close();
}
