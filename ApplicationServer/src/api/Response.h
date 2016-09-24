#ifndef APPLICATIONSERVER_RESPONSE_H
#define APPLICATIONSERVER_RESPONSE_H

#include <string>
#include "../jsoncpp/json/json.h"
#include "../jsoncpp/json/json-forwards.h"

using namespace std;

class Response {
public:
    Response();
    ~Response();

    const char* getHeader();
    const char* getBody();
    unsigned long getBodyLength();
    void setBody(string body);
    void setErrorBody(string errorDetails);
    void setSuccessfulBody();

    void setSuccessfulHeader();
    void setNotFoundHeader();
    void setBadRequestHeader();
    void setNotImplementedHeader();
private:
    string header;
    string body;
    unsigned long bodyLength;
};

#endif //APPLICATIONSERVER_RESPONSE_H
