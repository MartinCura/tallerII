#ifndef PROJECT_MESSAGE_H
#define PROJECT_MESSAGE_H

#include "../../jsoncpp/json/json.h"

using namespace std;

class Message {

public:
    static const char* TIMESTAMP_FORMAT;

    Message(Json::Value messageAsJson);
    Message();
    ~Message();

    void setMessage(string message);
    void setTimestamp(string timestamp);
    void setFromUserId(long fromUserId);
    void setToUserId(long toUserId);

    string getMessage();
    string getTimestamp();
    long getFromUserId();
    long getToUserId();

    Json::Value serializeMe();

private:
    string timestamp;
    string message;
    long fromUserId;
    long toUserId;
};

#endif //PROJECT_MESSAGE_H
