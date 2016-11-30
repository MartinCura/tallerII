#ifndef PROJECT_ERRORDELETINGFILEEXCEPTION_H
#define PROJECT_ERRORDELETINGFILEEXCEPTION_H

#include <string>

using namespace std;

class ErrorDeletingFileException: public exception{
public:
    ErrorDeletingFileException(string message) {
        this->message = message;
    }
    ~ErrorDeletingFileException() {}

    string getMessage() const {
        return this->message;
    }

private:
    string message;
};

#endif //PROJECT_ERRORDELETINGFILEEXCEPTION_H
