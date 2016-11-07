#include "NotificationTokenManager.h"

const string NotificationTokenManager::TOKEN_KEY = "token";
const string TOKEN_KEY_FOR_DATABASE = "notifications_token_";

NotificationTokenManager::NotificationTokenManager(DBWrapper* db) {
    this->db = db;
}

NotificationTokenManager::~NotificationTokenManager() {}

string NotificationTokenManager::getTokenByUserId(long userId) {
    string key = TOKEN_KEY_FOR_DATABASE + to_string(userId);
    string result;
    if (this->db->existsKey(key, &result)) {
        return result;
    }
    throw NonexistentNotificationToken(userId);
}

void NotificationTokenManager::setOrUpdateToken(long userId, string token) {
    string key = TOKEN_KEY_FOR_DATABASE + to_string(userId);
    this->db->puTKey(key, &token);
}
