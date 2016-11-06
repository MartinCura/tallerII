#ifndef PROJECT_NONEXISTENTNOTIFICATIONTOKEN_H
#define PROJECT_NONEXISTENTNOTIFICATIONTOKEN_H

#include <exception>

class NonexistentNotificationToken : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Nonexistent Token";
    }

};

#endif //PROJECT_NONEXISTENTNOTIFICATIONTOKEN_H
