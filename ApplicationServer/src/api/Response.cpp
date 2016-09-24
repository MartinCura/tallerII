#include "Response.h"

Response::Response() {
    this->header = "";
    this->body = "";
    this->bodyLength = 0;
}

Response::~Response() {}

const char *Response::getHeader() {
    string fullHeader = "HTTP/1.1 " + this->header + "\r\n";
    fullHeader += "Transfer-Encoding: chunked\r\n";
    fullHeader += "Content-Length: " + to_string(this->bodyLength) + "\r\n";
    fullHeader += "\r\n";
    return fullHeader.c_str();
}

const char *Response::getBody() {
    return this->body.c_str();
}

unsigned long Response::getBodyLength() {
    return this->bodyLength;
}

void Response::setBody(string body) {
    this->body = body;
    this->bodyLength = body.length();
}

void Response::setErrorBody(string errorDetails) {
    Json::Value root;
    root["successful"] = "false";
    root["error"] = errorDetails;
    this->setBody(root.toStyledString());
}

void Response::setSuccessfulBody() {
    Json::Value root;
    root["successful"] = "true";
    this->setBody(root.toStyledString());
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
