#ifndef PROJECT_INVALIDREQUESTEXCEPTION_H
#define PROJECT_INVALIDREQUESTEXCEPTION_H

#include <string>

using namespace std;

class InvalidRequestException {
public:
    InvalidRequestException(const string& message) : errorMessage(message) {}
    ~InvalidRequestException() {}

    string getMessage() const {
        return(errorMessage);
    }

private:
    string errorMessage;
};

#endif //PROJECT_INVALIDREQUESTEXCEPTION_H
