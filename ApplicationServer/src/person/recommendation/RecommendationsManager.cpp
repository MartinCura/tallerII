#include "RecommendationsManager.h"
#include "../Person.h"

const string RECOMMENDATION_KEY = "recommendation_";
const string ARRAY_KEY = "recommendations";
#define USER_UUID_ID "user:uuid_"
#define USER_MAIL_ID "user:mail_"

RecommendationsManager::RecommendationsManager(DBWrapper* db) {
    this->db = db;
}

RecommendationsManager::~RecommendationsManager() {}


void RecommendationsManager::addRecommendation(long fromId, long toId) {
    vector<long> currentRecommendations = this->getRecommendationsByUserId(toId);
    if (!this->recommendationAlreadyExists(currentRecommendations, fromId)) {
        currentRecommendations.push_back(fromId);
        Json::Value recommendationsAsJson = this->buildJsonToSave(currentRecommendations);
        Json::FastWriter fastWriter;
        string valueToSave = fastWriter.write(recommendationsAsJson);
        this->db->puTKey(RECOMMENDATION_KEY + to_string(toId), &valueToSave);
        updateTotalRecommendation(1, toId);
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
        Json::Value recommendationsAsJson = this->buildJsonToSave(newRecommendations);
        Json::FastWriter fastWriter;
        string valueToSave = fastWriter.write(recommendationsAsJson);
        this->db->puTKey(RECOMMENDATION_KEY + to_string(toId), &valueToSave);
        updateTotalRecommendation(-1, toId);
    }
}

vector<long> RecommendationsManager::getRecommendationsByUserId(long userId) {
    vector<long> recommendations;
    std::string result;
    if (!this->db->existsKey(RECOMMENDATION_KEY + to_string(userId), &result)) {
        return recommendations;
    }
    recommendations = this->parseRecommendationsString(result);
    return recommendations;
}

Json::Value RecommendationsManager::getRecommendationsAsJson(long userId) {
    vector<long> recommendations = this->getRecommendationsByUserId(userId);
    Json::Value recommendationsAsJson = Json::Value(Json::arrayValue);
    for (vector<long>::size_type i = 0; i < recommendations.size(); i++) {
        recommendationsAsJson.append(recommendations[i]);
    }
    return recommendationsAsJson;
}

bool RecommendationsManager::recommendationAlreadyExists(vector<long> currentRecommendations, long fromId) {
    for (vector<long>::size_type i = 0; i < currentRecommendations.size(); i++) {
        if (currentRecommendations[i] == fromId) {
            return true;
        }
    }
    return false;
}

Json::Value RecommendationsManager::buildJsonToSave(vector<long> currentRecommendations) {
    Json::Value recommendationsAsJson;
    for (vector<long>::size_type i = 0; i < currentRecommendations.size(); i++) {
        Json::Value item;
        item[to_string(i)] = currentRecommendations[i];
        recommendationsAsJson[ARRAY_KEY].append(item);
    }
    return recommendationsAsJson;
}

vector<long> RecommendationsManager::parseRecommendationsString(string recommendationsAsString) {
    vector<long> recommendations;
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(recommendationsAsString, root);
    if (!parsingSuccessful) {
        throw exception();
    }
    Json::Value recommendationsAsJson = root[ARRAY_KEY];
    for(int i = 0; i < recommendationsAsJson.size(); i++) {
        long recommendationItem = recommendationsAsJson[i][to_string(i)].asLargestInt();
        recommendations.push_back(recommendationItem);
    }
    return recommendations;
}

//Fixme: ver si esto va en PersonManager o aca.
void RecommendationsManager::updateTotalRecommendation(int delta, long id) {
    string output, user_information;
    this->db->getKey(USER_UUID_ID + std::to_string(id), &output);
    this->db->getKey(USER_MAIL_ID + output, &user_information);

    Json::Reader reader;
    Json::Value juser_info;
    reader.parse(user_information.c_str(), juser_info);

    Person* person = new Person(juser_info);
    int tot_recommendations = person->getTotalOfRecommendations();
    person->setTotalRecommendations(tot_recommendations + delta);

    Json::FastWriter fastWriter;
    string updated_user = fastWriter.write(person->serializeMe());
    this->db->puTKey(USER_MAIL_ID + output, &updated_user );
    delete person;


}
