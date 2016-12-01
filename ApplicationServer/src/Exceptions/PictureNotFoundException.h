#ifndef PROJECT_PICTURENOTFOUNDEXCEPTION_H
#define PROJECT_PICTURENOTFOUNDEXCEPTION_H

#include <string>

using namespace std;

class PictureNotFoundException: public exception{
public:
    PictureNotFoundException(string message) {
        this->message = message;
    }
    ~PictureNotFoundException() {}

    string getMessage() const {
        return this->message;
    }

private:
    string message;
};

#endif //PROJECT_PICTURENOTFOUNDEXCEPTION_H
