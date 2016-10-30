#include <iostream>
#include "Handler.h"
#include "../../session/SessionManager.h"

static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_delete_method = MG_MK_STR("DELETE");
static const struct mg_str s_post_method = MG_MK_STR("POST");

Handler::Handler() {}

Handler::~Handler() {}

Response* Handler::handleRequest(http_message* httpMessage, string url) {
    if (isPublic == false ) {
        //Se necesita verificar primero que la sesión esté abierta

        Json::Value b = httpMessage->header_values->p;
        std::string token;
        /*Json::Value body = this->parseBody(string(httpMessage->body.p));
        if (!body.isMember("token")) {
            throw InvalidRequestException("Token missing");
        }

        token = body["token"].asString();
        SessionManager* sessionManager = new SessionManager("/tmp/appDB");

        //si no falla es porque la sesión esta abierta.
        //TODO: VERIFICAR COMO AGREGAR LA INFORMACIÓN AL BODY
        body["user_mail_id"] = sessionManager->checkSession(token);
        httpMessage->body.p = body.toStyledString().c_str();*/
    }

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

long Handler::getUserIdFromUrl(string url) {
    vector<string> parsedUrl = this->parseUrl(url);
    if (parsedUrl.size() != 1) {
        throw InvalidRequestException("Cannot get user id from url.");
    }
    string userIdAsString = parsedUrl[0];
    try {
        long userId = stol(userIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}

string Handler::getStringFromMgStr(const struct mg_str structMgStr) {
    string asString = "";
    const char* pointer = structMgStr.p;
    for(int i = 0; i < structMgStr.len; i++) {
        asString = asString + *pointer;
        pointer++;
    }
    return asString;
}

string Handler::getParameterFromQueryParams(string queryParams, string parameter) {
    size_t found = queryParams.find(parameter);
    if (found == string::npos) {
        throw InvalidRequestException("Missing " + parameter + " parameter");
    }
    if ((found + parameter.length() + 2) > queryParams.length()) {
        throw InvalidRequestException("Invalid query params");
    }
    return queryParams.substr(found + parameter.length() + 1, found + parameter.length() + 2);
}
