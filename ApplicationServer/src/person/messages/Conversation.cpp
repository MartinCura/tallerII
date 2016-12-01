#include "Conversation.h"

const char *Conversation::WITH_KEY = "with";
const char *Conversation::FIRST_NAME_KEY = "first_name";
const char *Conversation::LAST_NAME_KEY = "last_name";
const char *Conversation::UNREAD_COUNT_KEY = "unread_count";

Conversation::Conversation() {
    this->with = 0;
    this->firstName = "";
    this->lastName = "";
    this->unreadCount = 0;
}

Conversation::~Conversation() {}

void Conversation::setWith(long with) {
    this->with = with;
}

void Conversation::setFirstName(string firstName) {
    this->firstName = firstName;
}

void Conversation::setLastName(string lastName) {
    this->lastName = lastName;
}

void Conversation::setUnreadCount(int unreadCount) {
    this->unreadCount = unreadCount;
}

long Conversation::getWith() {
    return this->with;
}

string Conversation::getFirstName(){
    return this->firstName;
}

string Conversation::getLastName() {
    return this->lastName;
}

int Conversation::getUnreadCount() {
    return this->unreadCount;
}

Json::Value Conversation::serializeMe() {
    Json::Value conversationAsJson;
    conversationAsJson[WITH_KEY] = this->getWith();
    conversationAsJson[FIRST_NAME_KEY] = this->getFirstName();
    conversationAsJson[LAST_NAME_KEY] = this->getLastName();
    conversationAsJson[UNREAD_COUNT_KEY] = this->getUnreadCount();
    return conversationAsJson;
}
