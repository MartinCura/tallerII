#include "WebHandler.h"
#include "handler/LoginHandler.h"

WebHandler::WebHandler() {}

WebHandler::~WebHandler() {}

Response* WebHandler::handleRequest(http_message* httpMessage) {
    Response* response = new Response();
    try {
        if (&httpMessage->uri) {
            string url = this->getUrl(httpMessage->uri);
            if (regex_match(url, regex("/users/.*")) || regex_match(url, regex("/users"))) {
                this->logRequest(httpMessage);
                UsersHandler* handler = new UsersHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                this->logResponse(response);
                return response;
            }
            if (regex_match(url, regex("/profilepicture/.*"))) {
                this->logRequest(httpMessage, false);
                PictureHandler* handler = new PictureHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response, false);
                delete handler;
                return response;
            }
            if (regex_match(url, regex("/allusers"))) {
                this->logRequest(httpMessage);
                AllUsersHandler* handler = new AllUsersHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                this->logResponse(response);
                return response;
            }
            if (regex_match(url, regex("/contacts/.*")) || regex_match(url, regex("/contacts"))) {
                this->logRequest(httpMessage);
                ContactsHandler *handler = new ContactsHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                this->logResponse(response);
                return response;
            }
            if (regex_match(url, regex("/login"))) {
                this->logRequest(httpMessage);
                LoginHandler* handler = new LoginHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                this->logResponse(response);
                return response;
            }
            if (regex_match(url, regex("/recommendations/.*")) || regex_match(url, regex("/recommendations"))) {
                this->logRequest(httpMessage);
                RecommendationsHandler *handler = new RecommendationsHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                this->logResponse(response);
                return response;
            }
            if (regex_match(url, regex("/messages/.*")) || regex_match(url, regex("/messages"))) {
                this->logRequest(httpMessage);
                MessagesHandler *handler = new MessagesHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                this->logResponse(response);
                return response;
            }
        }
        response->setNotFoundHeader();
    } catch (exception &e) {
        response->setInternalServerErrorHeader();
        response->setErrorBody(e.what());
    }
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

void WebHandler::logRequest(http_message* httpMessage, bool logBody) {
    Logger::getInstance()->debug(this->getStringFromMgStr(httpMessage->method) + " request recibido");
    Logger::getInstance()->debug("URL: " + this->getStringFromMgStr(httpMessage->uri));
    string body = logBody ? this->getStringFromMgStr(httpMessage->body) : "**Body no logueado**";
    Logger::getInstance()->debug("Body: " + body);
}

void WebHandler::logResponse(Response* response, bool logBody) {
   Logger::getInstance()->debug("Response header: " + string(response->getHeader()));
   string body = logBody ? string(response->getBody()) : "**Body no logueado**";
   Logger::getInstance()->debug("Response body: " + body);
}
