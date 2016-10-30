#include "Message.h"

Message::Message(Json::Value messageAsJson) {
    this->message = messageAsJson["message"].asString();
    this->timestamp = messageAsJson["timestamp"].asString();
    this->fromUserId = messageAsJson["from"].asLargestInt();
    this->toUserId = messageAsJson["to"].asLargestInt();
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

Json::Value Message::serializeMe() {
    Json::Value messageAsJson;
    messageAsJson["from"] = this->fromUserId;
    messageAsJson["to"] = this->toUserId;
    messageAsJson["message"] = this->message;
    messageAsJson["timestamp"] = this->timestamp;
    return messageAsJson;
}