#ifndef PROJECT_CONTACT_H
#define PROJECT_CONTACT_H

#include "../../jsoncpp/json/json.h"

using namespace std;

class Contact {

public:
    Contact(Json::Value contactAsJson);
    Contact();
    ~Contact();

    void setStatus(string status);
    void setUserId(long userId);

    string getStatus();
    long getUserId();

private:
    string status;
    long userId;
};

#endif //PROJECT_CONTACT_H
