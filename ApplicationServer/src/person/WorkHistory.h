#ifndef PROJECT_WORKHISTORY_H
#define PROJECT_WORKHISTORY_H

#include <string>
#include "json/json.h"
#include "json/json-forwards.h"


using namespace std;

class WorkHistory {

public:
    WorkHistory();
    WorkHistory(Json::Value jvalue);

    virtual ~WorkHistory();

    void setPositionTitle(string positionTitle);
    void setCompany(string company);
    void setFromDate(string fromDate);
    void setToDate(string toDate);

    string getPositionTitle();
    string getCompany();
    string getFromDate();
    string getToDate();

    Json::Value serializeMe();

private:
    string positionTitle;
    string company;
    string fromDate;
    string toDate;

    /// Creates an object WorkHistory from a Json
    void deserializeMe(Json::Value jvalue);
};

#endif //PROJECT_WORKHISTORY_H
