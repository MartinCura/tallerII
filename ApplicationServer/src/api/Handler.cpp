#include "Handler.h"

static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_delete_method = MG_MK_STR("DELETE");
static const struct mg_str s_post_method = MG_MK_STR("POST");

Handler::Handler() {}

Handler::~Handler() {}

Response* Handler::handleRequest(http_message* httpMessage, string url) {
    if (this->isEqual(&httpMessage->method, &s_get_method)) {
        return this->handleGetRequest(httpMessage, url);
    } else if (this->isEqual(&httpMessage->method, &s_put_method)) {
        return this->handlePutRequest(httpMessage);
    } else if (this->isEqual(&httpMessage->method, &s_delete_method)) {
        return this->handleDeleteRequest(httpMessage);
    } else if (this->isEqual(&httpMessage->method, &s_post_method)) {
        return this->handlePostRequest(httpMessage);
    }
    Response* response = new Response();
    response->setNotImplementedHeader();
    return response;
}

Response* Handler::getNotImplementedResponse() {
    Response* response = new Response();
    response->setNotImplementedHeader();
    return response;
}

Response* Handler::getSuccessfulResponse() {
    Response* response = new Response();
    response->setSuccessfulHeader();
    response->setSuccessfulBody();
    return response;
}

Response* Handler::getBadRequestResponse(string errorDetails) {
    Response* response = new Response();
    response->setBadRequestHeader();
    response->setErrorBody(errorDetails);
    return response;
}

Response* Handler::getNotFoundResponse(string errorDetails) {
    Response* response = new Response();
    response->setNotFoundHeader();
    response->setErrorBody(errorDetails);
    return response;
}

int Handler::isEqual(const struct mg_str *s1, const struct mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

Json::Value Handler::parseBody(string body) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(body, root);
    if (!parsingSuccessful) {
        throw "Invalid Json.";
    }
    return root;
}
