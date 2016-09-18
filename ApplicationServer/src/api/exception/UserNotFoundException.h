#ifndef PROJECT_USERNOTFOUNDEXCEPTION_H
#define PROJECT_USERNOTFOUNDEXCEPTION_H

#include <string>

using namespace std;

class UserNotFoundException {
public:
    UserNotFoundException(int userId) {
        this->userId = userId;
    }
    ~UserNotFoundException() {}

    string getMessage() const {
        return(string ("User id " + to_string(this->userId) + " doesn't exist"));
    }

private:
    int userId;
};

#endif //PROJECT_USERNOTFOUNDEXCEPTION_H
