#include "RecommendationsManager.h"

const string RECOMMENDATION_KEY = "recommendation_";
const string DELIMITER = "-";

RecommendationsManager::RecommendationsManager(DBWrapper* db) {
    this->db = db;
}

RecommendationsManager::~RecommendationsManager() {}


void RecommendationsManager::addRecommendation(long fromId, long toId) {
    vector<long> currentRecommendations = this->getRecommendationsByUserId(toId);
    if (!this->recommendationAlreadyExists(currentRecommendations, fromId)) {
        currentRecommendations.push_back(fromId);
        string allRecommendationsAsString = this->buildStringToSave(currentRecommendations);
        Json::FastWriter fastWriter;
        string valueToSave = fastWriter.write(allRecommendationsAsString);
        this->db->puTKey(RECOMMENDATION_KEY + to_string(toId), &valueToSave);
    }
}

void RecommendationsManager::removeRecommendation(long fromId, long toId) {
    vector<long> currentRecommendations = this->getRecommendationsByUserId(toId);
    if (this->recommendationAlreadyExists(currentRecommendations, fromId)) {
        vector<long> newRecommendations;
        for (vector<long>::size_type i = 0; i < currentRecommendations.size(); i++) {
            if (currentRecommendations[i] != fromId) {
                newRecommendations.push_back(currentRecommendations[i]);
            }
        }
        string allRecommendationsAsString = this->buildStringToSave(newRecommendations);
        Json::FastWriter fastWriter;
        string valueToSave = fastWriter.write(allRecommendationsAsString);
        this->db->puTKey(RECOMMENDATION_KEY + to_string(toId), &valueToSave);
    }
}

vector<long> RecommendationsManager::getRecommendationsByUserId(long userId) {
    vector<long> recommendations;
    std::string result;
    if (!this->db->existsKey(RECOMMENDATION_KEY + to_string(userId), &result)) {
        return recommendations;
    }
    //TODO: FALTA AGREGAR LOGICA
    return recommendations;
}

bool RecommendationsManager::recommendationAlreadyExists(vector<long> currentRecommendations, long fromId) {
    for (vector<long>::size_type i = 0; i < currentRecommendations.size(); i++) {
        if (currentRecommendations[i] == fromId) {
            return true;
        }
    }
    return false;
}

string RecommendationsManager::buildStringToSave(vector<long> currentRecommendations) {
    string allRecommendationsAsString = "";
    for (vector<long>::size_type i = 0; i < currentRecommendations.size(); i++) {
        allRecommendationsAsString = allRecommendationsAsString + to_string(currentRecommendations[i]);
        if (i != (currentRecommendations.size() - 1)) {
            allRecommendationsAsString = allRecommendationsAsString + DELIMITER;
        }
    }
}
