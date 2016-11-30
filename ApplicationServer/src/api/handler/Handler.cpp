#include <iostream>
#include "Handler.h"
#include "../../session/SessionManager.h"
#include "../../Exceptions/NotAuthorizedException.h"

static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_delete_method = MG_MK_STR("DELETE");
static const struct mg_str s_post_method = MG_MK_STR("POST");

Handler::Handler() {
    this->namedb = new string("/tmp/appDB");
    db = DBWrapper::openDb(namedb);

}

Handler::~Handler() {
    delete session;
    delete namedb;
    DBWrapper::ResetInstance();
}

Response* Handler::handleRequest(http_message* httpMessage, string url) {
    try {
        if (this->isEqual(&httpMessage->method, &s_get_method)) {
            if (!getPublic) session = controlAccess(httpMessage);
            return this->handleGetRequest(httpMessage, url);
        } else if (this->isEqual(&httpMessage->method, &s_put_method)) {
            if (!putPublic) session = controlAccess(httpMessage);
            return this->handlePutRequest(httpMessage, url);
        } else if (this->isEqual(&httpMessage->method, &s_delete_method)) {
            if (!deletePublic) session = controlAccess(httpMessage);
            return this->handleDeleteRequest(httpMessage, url);
        } else if (this->isEqual(&httpMessage->method, &s_post_method)) {
            if (!postPublic) session = controlAccess(httpMessage);
            return this->handlePostRequest(httpMessage);
        }
    }catch (InvalidRequestException& e) {
        Response* response = new Response();
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
        return response;
    } catch (NotAuthorizedException e) {
        Response* response = new Response();
        response->setForbiddenRequestHeader();
        response->setErrorBody(e.what());
        return response;
    } catch (exception& e) {
        Response* response = new Response();
        response->setUnauthorizedHeader();
        response->setBody(e.what());
        return response;
    }
    Response* response = new Response();
    response->setNotImplementedHeader();
    return response;
}

Session * Handler::controlAccess(http_message *httpMessage) {
    std::string token = this->getHttpHeader(httpMessage, "Authorization");
    if (token == "") throw InvalidRequestException("Falta el token de autenticación en el header");
    SessionManager* sessionManager = new SessionManager(this->db);
    Session* session = nullptr;
    try {
        session = sessionManager->getSession(token);
    } catch (exception& e){
        delete sessionManager;
        throw;
    }
    delete sessionManager;
    return session;

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

string Handler::getHttpHeader(http_message *message, string name) {
    const struct mg_str *header_information = mg_get_http_header(message, name.c_str());

    if (header_information == NULL) {
        return "";
    }

    const char *header_message = header_information->p;

    size_t len = header_information->len;
    char header_value[len + 1];

    memcpy(header_value, &header_message[0], len);
    header_value[len] = '\0';
    return string(header_value);

}
