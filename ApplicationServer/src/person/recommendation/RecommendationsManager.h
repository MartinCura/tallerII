#ifndef PROJECT_RECOMMENDATIONSMANAGER_H
#define PROJECT_RECOMMENDATIONSMANAGER_H

#include "../../DB/DBWrapper.h"
#include "../../Exceptions/ErrorOpeningDatabaseException.h"
#include "../../Exceptions/InvalidRequestException.h"
#include "../../jsoncpp/json/json.h"

using namespace std;

class RecommendationsManager {

public:
    RecommendationsManager(DBWrapper* db);
    ~RecommendationsManager();

    void addRecommendation(long fromId, long toId);
    void removeRecommendation(long fromId, long toId);
    vector<long> getRecommendationsByUserId(long userId);

private:
    DBWrapper* db;

    bool recommendationAlreadyExists(vector<long> currentRecommendations, long fromId);
    string buildStringToSave(vector<long> currentRecommendations);
};

#endif //PROJECT_RECOMMENDATIONSMANAGER_H
