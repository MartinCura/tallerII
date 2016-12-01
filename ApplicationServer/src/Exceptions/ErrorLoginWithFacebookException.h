#ifndef PROJECT_ERRORLOGINWITHFACEBOOKEXCEPTION_H
#define PROJECT_ERRORLOGINWITHFACEBOOKEXCEPTION_H

#include <string>

using namespace std;

class ErrorLoginWithFacebookException: public exception{
public:
    ErrorLoginWithFacebookException(string message) {
        this->message = message;
    }
    ~ErrorLoginWithFacebookException() {}

    string getMessage() const {
        return this->message;
    }

private:
    string message;
};

#endif //PROJECT_ERRORLOGINWITHFACEBOOKEXCEPTION_H
