#include "WebHandler.h"
#include "handler/SearchHandler.h"

WebHandler::WebHandler() {}

WebHandler::~WebHandler() {}

Response* WebHandler::handleRequest(http_message* httpMessage) {
    Handler* handler;
    try {
        if (&httpMessage->uri) {
            string url = this->getUrl(httpMessage->uri);
            if (regex_match(url, regex("/users/.*")) || regex_match(url, regex("/users"))) {
                this->logRequest(httpMessage);
                handler = new UsersHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/profilepicture/.*"))) {
                handler = new PictureHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/contacts/.*")) || regex_match(url, regex("/contacts"))) {
                this->logRequest(httpMessage);
                handler = new ContactsHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/login"))) {
                this->logRequest(httpMessage);
                handler = new LoginHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/facebooklogin"))) {
                this->logRequest(httpMessage);
                handler = new LoginWithFacebookHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/recommendations.*"))) {
                this->logRequest(httpMessage);
                handler = new RecommendationsHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/messages/.*")) || regex_match(url, regex("/messages"))) {
                this->logRequest(httpMessage);
                handler = new MessagesHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/conversations/.*"))) {
                this->logRequest(httpMessage);
                handler = new ConversationsHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/notificationtokens/.*"))) {
                this->logRequest(httpMessage);
                handler = new NotificationTokenHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/search.*"))) {
                this->logRequest(httpMessage);
                handler = new SearchHandler();
                Response* response = handler->handleRequest(httpMessage, url);
                this->logResponse(response);
                delete handler;
                return response;
            }
            else if (regex_match(url, regex("/profile.*"))) {
                this->logRequest(httpMessage);
                throw SpecialRequestException();
            }
            else {
                Response* response = new Response();
                this->logRequest(httpMessage);
                response->setNotFoundHeader();
                this->logResponse(response);
                return response;
            }
        }
    } catch (SpecialRequestException &e) {
        throw e;
    } catch (exception &e) {
        Response* response = new Response();
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
