#include "UsersHandler.h"
#include "../../Security/Security.h"
#include "../../Exceptions/NotAuthorizedException.h"



UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    string requestBody = string(httpMessage->body.p);
    PersonManager *personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        Json::Value responseBody;
        Json::Value parsedBody = this->parseBody(requestBody);
        responseBody["id"] = personManager->savePerson(parsedBody);
        response->setSuccessfulHeader();
        response->setBody(responseBody.toStyledString());
    } catch (UserAlreadyExistsException& e) {
        response->setConflictHeader();
        response->setErrorBody(e.what());
    } catch (InvalidRequestException& e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    }
    delete personManager;
    return response;
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    PersonManager *personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        Person *person = personManager->getUserById(userId);
        response->setSuccessfulHeader();
        person->setPassword("");
        Json::Value body = person->serializeMe();
        body["recommendations"] = personManager->getRecommendationsByUserId(userId);
        response->setBody(body.toStyledString());
        delete person;
    } catch (InvalidRequestException& e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    } catch (UserNotFoundException& e) {
        response->setNotFoundHeader();
        response->setErrorBody(e.getMessage(UserNotFoundException::Message::id));
    }
    delete personManager;
    return response;
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    PersonManager *personManager = new PersonManager(this->db);
    Response* response = new Response();
    try {
        long userId = this->getUserIdFromUrl(url);
        //Seguridad:
        // El usuario solo puede eliminar su perfil.
        if (!Security::hasPermissionToDeleteUser(this->session->getUserId(), userId)) {
            throw NotAuthorizedException();
        }
        personManager->deletePerson(userId);
        response->setSuccessfulHeader();
    } catch (InvalidRequestException& e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    } catch (UserNotFoundException& e) {
        response->setNotFoundHeader();
        response->setErrorBody(e.getMessage(UserNotFoundException::Message::id));
    }
    delete personManager;
    return response;
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage, string url) {
    PersonManager *personManager = new PersonManager(this->db);
    Response* response = new Response();
    string requestBody = string(httpMessage->body.p);

    long userId = this->getUserIdFromUrl(url);
    //Seguridad:
    // El usuario solo puede editar su perfil.
    if (!Security::hasPermissionToEdit(this->session->getUserId(), userId)) {
        throw NotAuthorizedException();
    }

    try {
        Json::Value parsedBody = this->parseBody(requestBody);

        //Person* person = personManager->getUserById(userId);
        //person->updateMe(parsedBody);
        //Json::Value jperson = person->serializeMe();
        //personManager->updateUser(jperson);
        //delete person;
        Json::Value responseBody;
        parsedBody["id"] = userId;
        responseBody["id"] = personManager->updateUser(parsedBody);

        response->setSuccessfulHeader();
        response->setBody(responseBody.toStyledString());
    } catch (InvalidRequestException& e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    } catch (UserNotFoundException& e) {
        response->setNotFoundHeader();
        response->setErrorBody(e.getMessage(UserNotFoundException::Message::id));
    }
    delete personManager;
    return response;
}
