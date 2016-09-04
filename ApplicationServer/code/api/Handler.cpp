#include "Handler.h"

static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_delete_method = MG_MK_STR("DELETE");
static const struct mg_str s_post_method = MG_MK_STR("POST");

Handler::Handler() {}

Handler::~Handler() {}

Response* Handler::handleRequest(http_message* httpMessage, string url) {
    if (this->isEqual(&httpMessage->method, &s_get_method)) {
        return this->handleGetRequest(httpMessage);
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

int Handler::isEqual(const struct mg_str *s1, const struct mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}
