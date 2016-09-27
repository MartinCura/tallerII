#ifndef PROJECT_USERNOTFOUNDEXCEPTION_H
#define PROJECT_USERNOTFOUNDEXCEPTION_H

#include <string>

using namespace std;

class UserNotFoundException: public exception{
public:

    enum Message {id, mail};

    UserNotFoundException(int userId) {
        this->userId = userId;
    }

    UserNotFoundException(std::string userMail) {
        this->userMail = userMail;
    }
    ~UserNotFoundException() {}

    string getMessage(Message option) const {
        switch (option) {
            case id:
                return (string("User id " + to_string(this->userId) + " doesn't exist"));
            case mail:
            default:
                return (string("User mail " + this->userMail + "doesn't exist"));
        }

    }

private:
    int userId;
    string userMail;
};

#endif //PROJECT_USERNOTFOUNDEXCEPTION_H
