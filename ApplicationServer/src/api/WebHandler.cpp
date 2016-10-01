#include "WebHandler.h"

WebHandler::WebHandler() {}

WebHandler::~WebHandler() {}

Response* WebHandler::handleRequest(http_message* httpMessage) {
    Response* response = new Response();
    this->logRequest(httpMessage);
    if (&httpMessage->uri) {
        string url = this->getUrl(httpMessage->uri);
        if (regex_match(url, regex("/users/.*")) || regex_match(url, regex("/users"))) {
            UsersHandler* handler = new UsersHandler();
            response = handler->handleRequest(httpMessage, url);
            delete handler;
            this->logResponse(response);
            return response;
        }
        if (regex_match(url, regex("/profilepicture/.*"))) {
            PictureHandler* handler = new PictureHandler();
            response = handler->handleRequest(httpMessage, url);
            delete handler;
            return response;
        }
        if (regex_match(url, regex("/allusers"))) {
            AllUsersHandler* handler = new AllUsersHandler();
            response = handler->handleRequest(httpMessage, url);
            delete handler;
            return response;
        }
    }
    response->setNotFoundHeader();
    this->logResponse(response);
    return response;
}

string WebHandler::getUrl(const struct mg_str uri) {
    return this->getStringFromMgStr(uri);
}

string WebHandler::getStringFromMgStr(const struct mg_str structMgStr) {
    string asString = "";
    const char* pointer = structMgStr.p;
    for(int i = 0; i < structMgStr.len; i++) {
        asString = asString + *pointer;
        pointer++;
    }
    return asString;
}

void WebHandler::logRequest(http_message* httpMessage) {
    Logger::getInstance()->debug(this->getStringFromMgStr(httpMessage->method) + " request recibido");
    Logger::getInstance()->debug("URL: " + this->getStringFromMgStr(httpMessage->uri));
    Logger::getInstance()->debug("Body: " + this->getStringFromMgStr(httpMessage->body));
}

void WebHandler::logResponse(Response* response) {
   Logger::getInstance()->debug("Response header: " + string(response->getHeader()));
   Logger::getInstance()->debug("Response body: " + string(response->getBody()));
}
