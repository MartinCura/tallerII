#include "WebHandler.h"

WebHandler::WebHandler() {}

WebHandler::~WebHandler() {}

Response* WebHandler::handleRequest(http_message* httpMessage) {
    Response* response = new Response();
    Handler* handler;
    try {
        if (&httpMessage->uri) {
            string url = this->getUrl(httpMessage->uri);
            this->logRequest(httpMessage);
            if (regex_match(url, regex("/users/.*")) || regex_match(url, regex("/users"))) {
                handler = new UsersHandler();
                response = handler->handleRequest(httpMessage, url);
            }
            else if (regex_match(url, regex("/profilepicture/.*"))) {
                handler = new PictureHandler();
                response = handler->handleRequest(httpMessage, url);
            }
            else if (regex_match(url, regex("/allusers"))) {
                handler = new AllUsersHandler();
                response = handler->handleRequest(httpMessage, url);
            }
            else if (regex_match(url, regex("/contacts/.*")) || regex_match(url, regex("/contacts"))) {
                handler = new ContactsHandler();
                response = handler->handleRequest(httpMessage, url);
            }
            else if (regex_match(url, regex("/login"))) {
                handler = new LoginHandler();
                response = handler->handleRequest(httpMessage, url);
            }
            else if (regex_match(url, regex("/recommendations.*"))) {
                handler = new RecommendationsHandler();
                response = handler->handleRequest(httpMessage, url);
            }
            else if (regex_match(url, regex("/messages/.*")) || regex_match(url, regex("/messages"))) {
                handler = new MessagesHandler();
                response = handler->handleRequest(httpMessage, url);

            }
            else if (regex_match(url, regex("/notificationtokens/.*"))) {
                handler = new NotificationTokenHandler();
                response = handler->handleRequest(httpMessage, url);
            } else {
                response->setNotFoundHeader();
            }
        }
    } catch (exception &e) {
        response->setInternalServerErrorHeader();
        response->setErrorBody(e.what());
    }
    delete handler;
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
