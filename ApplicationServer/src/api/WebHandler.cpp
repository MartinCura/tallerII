#include "WebHandler.h"
#include <regex>

WebHandler::WebHandler() {}

WebHandler::~WebHandler() {}

Response* WebHandler::handleRequest(http_message* httpMessage, int ev) {
    if (&httpMessage->uri) {
        string url = this->getUrl(httpMessage->uri);
        if (regex_match(url, regex("/users/.*"))) {
            UsersHandler* handler = new UsersHandler();
            return handler->handleRequest(httpMessage, ev, url);
        }
    }
    Response* response = new Response();
    response->setNotFoundHeader();
    return response;
}

string WebHandler::getUrl(const struct mg_str uri) {
    string url = "";
    const char* pointer = uri.p;
    for(int i = 0; i < uri.len; i++) {
        url = url + *pointer;
        pointer++;
    }
    return url;
}
