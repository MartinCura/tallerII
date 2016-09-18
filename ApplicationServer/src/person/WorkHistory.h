#ifndef PROJECT_WORKHISTORY_H
#define PROJECT_WORKHISTORY_H

#include <string>

using namespace std;

class WorkHistory {

public:
    WorkHistory();
    virtual ~WorkHistory();

    void setPositionTitle(string positionTitle);
    void setCompany(string company);
    void setFromDate(string fromDate);
    void setToDate(string toDate);

    string getPositionTitle();
    string getCompany();
    string getFromDate();
    string getToDate();

private:
    string positionTitle;
    string company;
    string fromDate;
    string toDate;
};

#endif //PROJECT_WORKHISTORY_H
