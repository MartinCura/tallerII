#include "Contact.h"

Contact::Contact(Json::Value contactAsJson) {
    this->status = contactAsJson["status"].asString();
    this->userId = contactAsJson["user_id"].asLargestInt();
}

Contact::Contact() {}

Contact::~Contact() {}

void Contact::setStatus(string status) {
    this->status = status;
}

void Contact::setUserId(long userId) {
    this->userId = userId;
}

string Contact::getStatus() {
    return status;
}

long Contact::getUserId() {
    return userId;
}
