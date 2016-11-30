#ifndef PROJECT_NONEXISTENTNOTIFICATIONTOKEN_H
#define PROJECT_NONEXISTENTNOTIFICATIONTOKEN_H

#include <exception>
#include <string>

using namespace std;

class NonexistentNotificationToken : public exception {
public:
    NonexistentNotificationToken(long userId) {
        this->message = "No token for message notifications for user " + to_string(userId);
    }
    ~NonexistentNotificationToken() {}

    string getMessage() const {
        return this->message;
    }

private:
    string message;
};

#endif //PROJECT_NONEXISTENTNOTIFICATIONTOKEN_H
