#include "NotificationTokenManager.h"

const string NotificationTokenManager::TOKEN_KEY = "token";
const string TOKEN_KEY_FOR_DATABASE = "token_";

NotificationTokenManager::NotificationTokenManager(DBWrapper* db) {
    this->db = db;
}

NotificationTokenManager::~NotificationTokenManager() {}

string NotificationTokenManager::getTokenByUserId(long userId) {
    string key = TOKEN_KEY_FOR_DATABASE + to_string(userId);
    string result;
    if (this->db->existsKey(key, &result)) {
        Json::Reader reader;
        Json::Value token;
        reader.parse(result.c_str(), token);
        return token.asString();
    }
    throw new NonexistentNotificationToken();
}

void NotificationTokenManager::setOrUpdateToken(long userId, string token) {
    string key = TOKEN_KEY_FOR_DATABASE + to_string(userId);
    this->db->puTKey(key, &token);
}
