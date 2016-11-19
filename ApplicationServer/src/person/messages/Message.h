#ifndef PROJECT_MESSAGE_H
#define PROJECT_MESSAGE_H

#include "../../jsoncpp/json/json.h"

using namespace std;

class Message {

public:
    static const char* TIMESTAMP_FORMAT;
    static const char* STATE_SENT;
    static const char* STATE_RECEIVED;
    static const char* STATE_READ;

    Message(Json::Value messageAsJson);
    Message();
    ~Message();

    void setMessage(string message);
    void setTimestamp(string timestamp);
    void setFromUserId(long fromUserId);
    void setToUserId(long toUserId);
    void setState(string state);

    string getMessage();
    string getTimestamp();
    long getFromUserId();
    long getToUserId();
    string getState();

    Json::Value serializeMe();

private:
    string timestamp;
    string message;
    long fromUserId;
    long toUserId;
    string state;
};

#endif //PROJECT_MESSAGE_H
