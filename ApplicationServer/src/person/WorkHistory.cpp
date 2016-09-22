#include "WorkHistory.h"

WorkHistory::WorkHistory() {
    this->positionTitle = "";
    this->company = "";
    this->fromDate = "";
    this->toDate = "";
}

WorkHistory::WorkHistory(Json::Value jvalue) {
    deserializeMe(jvalue);
}

WorkHistory::~WorkHistory() {}

void WorkHistory::setPositionTitle(string positionTitle) {
    this->positionTitle = positionTitle;
}

void WorkHistory::setCompany(string company) {
    this->company = company;
}

void WorkHistory::setFromDate(string fromDate) {
    this->fromDate = fromDate;
}

void WorkHistory::setToDate(string toDate) {
    this->toDate = toDate;
}

string WorkHistory::getPositionTitle() {
    return positionTitle;
}

string WorkHistory::getCompany() {
    return company;
}

string WorkHistory::getFromDate() {
    return fromDate;
}

string WorkHistory::getToDate() {
    return toDate;
}

void WorkHistory::deserializeMe(Json::Value jvalue) {

    this->positionTitle = jvalue["position_title"].asString();
    this->company = jvalue["company"].asString();
    this->fromDate = jvalue["from_date"].asString();
    this->toDate = jvalue["to_date"].asString();
}

Json::Value WorkHistory::serializeMe() {
    Json::Value response;
    response["position_title"] = this->positionTitle;
    response["company"] = this->company;
    response["from_date"] = this->fromDate;
    response["to_date"] = this->toDate;

}