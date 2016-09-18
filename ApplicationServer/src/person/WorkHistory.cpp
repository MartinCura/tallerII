#include "WorkHistory.h"

WorkHistory::WorkHistory() {
    this->positionTitle = "";
    this->company = "";
    this->fromDate = "";
    this->toDate = "";
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
