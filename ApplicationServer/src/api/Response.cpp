#include "Response.h"

Response::Response() {
    this->header = "";
    this->body = "";
}

Response::~Response() {}

const char *Response::getHeader() {
    string fullHeader =
        "HTTP/1.1 " + this->header + "\r\n"
        "Transfer-Encoding: chunked\r\n" +
        "Content-Length: " + to_string(this->body.length()) + "\r\n"
        "\r\n";
    return fullHeader.c_str();
}

const char *Response::getBody() {
    return this->body.c_str();
}

void Response::setBody(const char* body) {
    this->body = body;
}

void Response::setErrorBody(string errorDetails) {
    Json::Value root;
    root["successful"] = "false";
    root["error"] = errorDetails;
    this->body = root.toStyledString().c_str();
}

void Response::setSuccessfulBody() {
    Json::Value root;
    root["successful"] = "true";
    this->body = root.toStyledString().c_str();
}

void Response::setSuccessfulHeader() {
    this->header = "200 OK";
}

void Response::setBadRequestHeader() {
    this->header = "400 Bad Request";
}

void Response::setNotFoundHeader() {
    this->header = "404 Not Found";
}

void Response::setNotImplementedHeader() {
    this->header = "501 Not Implemented";
}
