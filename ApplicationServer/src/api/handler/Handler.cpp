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
        return this->handlePutRequest(httpMessage, url);
    } else if (this->isEqual(&httpMessage->method, &s_delete_method)) {
        return this->handleDeleteRequest(httpMessage, url);
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
        throw InvalidRequestException("Invalid Json.");
    }
    return root;
}

vector<string> Handler::parseUrl(string url) {
    vector<string> parsedUrl;
    if (this->getSubUrl(url) != "") {
        parsedUrl.push_back(this->getSubUrl(url));
    }
    return parsedUrl;
}

string Handler::getSubUrl(string url) {
    size_t sp = url.find_first_of('/', 1);
    if (sp == string::npos || ((url.begin() + sp + 1) >= (url.begin() + url.size()))) {
        return "";
    }
    string subUrl(url.begin() + sp + 1, url.begin() + url.size());
    return subUrl;
}

int Handler::getUserIdFromUrl(string url) {
    vector<string> parsedUrl = this->parseUrl(url);
    if (parsedUrl.size() != 1) {
        throw InvalidRequestException("Cannot get user id from url.");
    }
    string userIdAsString = parsedUrl[0];
    try {
        int userId = stoi(userIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}
