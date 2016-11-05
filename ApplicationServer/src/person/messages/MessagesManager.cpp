#include "MessagesManager.h"

const string DELIMITER = "_";
const string SENT = "sent";
const string RECEIVED = "received";

MessagesManager::MessagesManager(DBWrapper* db) {
    this->db = db;
}

MessagesManager::~MessagesManager() {}

string MessagesManager::saveMessage(long fromUserId, long toUserId, string message) {
    Message* messageToSave = new Message();
    messageToSave->setFromUserId(fromUserId);
    messageToSave->setToUserId(toUserId);
    messageToSave->setMessage(message);
    messageToSave->setTimestamp(this->getTimestamp());
    string messageToSaveAsString = messageToSave->serializeMe().toStyledString();
    vector<Message*> currentMessages = this->getMessages(fromUserId, toUserId);
    currentMessages.push_back(messageToSave);
    Json::Value allMessagesAsJson;
    for (vector<Message *>::size_type i = 0; i < currentMessages.size(); i++) {
        Message* currentMessage = currentMessages[i];
        allMessagesAsJson.append(currentMessage->serializeMe());
        delete currentMessage;
    }
    Json::FastWriter fastWriter;
    string valueToSave = fastWriter.write(allMessagesAsJson);
    string key = this->getKey(fromUserId, toUserId);
    this->db->puTKey(key, &valueToSave);
    return messageToSaveAsString;
}

vector<Message*> MessagesManager::getMessages(long fromUserId, long toUserId) {
    vector<Message*> messages;
    string result;
    string key = this->getKey(fromUserId, toUserId);
    if (this->db->existsKey(key, &result)) {
        Json::Reader reader;
        Json::Value messagesAsJson;
        reader.parse(result.c_str(), messagesAsJson);
        for (int index = 0; index < messagesAsJson.size(); index++) {
            Message *message = new Message(messagesAsJson[index]);
            messages.push_back(message);
        }
    }
    return messages;
}

string MessagesManager::getKey(long fromUserId, long toUserId) {
    if (fromUserId < toUserId) {
        return to_string(fromUserId) + DELIMITER + to_string(toUserId);
    } else {
        return to_string(toUserId) + DELIMITER + to_string(fromUserId);
    }
}

string MessagesManager::getTimestamp() {
    time_t now = time(0);
    struct tm * timeInfo = localtime(&now);
    char buffer[30];
    strftime(buffer, 30, Message::TIMESTAMP_FORMAT, timeInfo);
    return string(buffer);
}
