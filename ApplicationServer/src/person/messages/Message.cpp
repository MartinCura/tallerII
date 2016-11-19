#include "Message.h"

const char *Message::TIMESTAMP_FORMAT = "%Y-%m-%d %H:%M:%S";
const char *Message::STATE_SENT = "sent";
const char *Message::STATE_RECEIVED = "received";
const char *Message::STATE_READ = "read";

Message::Message(Json::Value messageAsJson) {
    this->message = messageAsJson["message"].asString();
    this->timestamp = messageAsJson["timestamp"].asString();
    this->fromUserId = messageAsJson["from"].asLargestInt();
    this->toUserId = messageAsJson["to"].asLargestInt();
    this->state = messageAsJson["state"].asString();
}

Message::Message() {}

Message::~Message() {}

void Message::setMessage(string message) {
    this->message = message;
}

void Message::setTimestamp(string timestamp){
    this->timestamp = timestamp;
}

void Message::setFromUserId(long fromUserId) {
    this->fromUserId = fromUserId;
}

void Message::setToUserId(long toUserId) {
    this->toUserId = toUserId;
}

void Message::setState(string state) {
    this->state = state;
}

string Message::getMessage() {
    return this->message;
}

string Message::getTimestamp(){
    return this->timestamp;
}

long Message::getFromUserId() {
    return this->fromUserId;
}

long Message::getToUserId() {
    return this->toUserId;
}

string Message::getState() {
    return this->state;
}

Json::Value Message::serializeMe() {
    Json::Value messageAsJson;
    messageAsJson["from"] = this->fromUserId;
    messageAsJson["to"] = this->toUserId;
    messageAsJson["message"] = this->message;
    messageAsJson["timestamp"] = this->timestamp;
    messageAsJson["state"] = this->state;
    return messageAsJson;
}