#ifndef PROJECT_USERNOTFOUNDEXCEPTION_H
#define PROJECT_USERNOTFOUNDEXCEPTION_H

#include <string>

using namespace std;

class UserNotFoundException: public exception{
public:

    enum Message {id, mail};

    UserNotFoundException(long userId) {
        this->userId = userId;
    }

    UserNotFoundException(std::string userMail) {
        this->userMail = userMail;
    }
    ~UserNotFoundException() {}

    string getMessage(Message option) const {
        switch (option) {
            case id:
                return (string("Usuario con el id " + to_string(this->userId) + " no existe en el sistema"));
            case mail:
            default:
                return (string("Usuario con el mail " + this->userMail + " no existe en el sistema"));
        }

    }

private:
    long userId;
    string userMail;
};

#endif //PROJECT_USERNOTFOUNDEXCEPTION_H
