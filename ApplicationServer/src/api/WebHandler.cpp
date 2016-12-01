#include "WebHandler.h"
#include "handler/SearchHandler.h"

WebHandler::WebHandler() {}

WebHandler::~WebHandler() {}

Response* WebHandler::handleRequest(http_message* httpMessage) {
    Response* response = new Response();
    Handler* handler;
    try {
        if (&httpMessage->uri) {
            string url = this->getUrl(httpMessage->uri);
            if (regex_match(url, regex("/users/.*")) || regex_match(url, regex("/users"))) {
                this->logRequest(httpMessage);
                handler = new UsersHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/profilepicture/.*"))) {
                handler = new PictureHandler();
                response = handler->handleRequest(httpMessage, url);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/contacts/.*")) || regex_match(url, regex("/contacts"))) {
                this->logRequest(httpMessage);
                handler = new ContactsHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/login"))) {
                this->logRequest(httpMessage);
                handler = new LoginHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/facebooklogin"))) {
                this->logRequest(httpMessage);
                handler = new LoginWithFacebookHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/recommendations.*"))) {
                this->logRequest(httpMessage);
                handler = new RecommendationsHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/messages/.*")) || regex_match(url, regex("/messages"))) {
                this->logRequest(httpMessage);
                handler = new MessagesHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/conversations/.*"))) {
                this->logRequest(httpMessage);
                handler = new ConversationsHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/notificationtokens/.*"))) {
                this->logRequest(httpMessage);
                handler = new NotificationTokenHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            } else if (regex_match(url, regex("/search.*"))) {
                this->logRequest(httpMessage);
                handler = new SearchHandler();
                response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }else {
                this->logRequest(httpMessage);
                response->setNotFoundHeader();
                this->logResponse(response);
                return response;
            }
        }
    } catch (exception &e) {
        this->logRequest(httpMessage);
        response->setInternalServerErrorHeader();
        response->setErrorBody(e.what());
        this->logResponse(response);
        return response;
    }
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
