//
// Created by milena on 02/10/16.
//

#ifndef APPLICATIONSERVER_TOKENEXPIREDEXCEPTION_H
#define APPLICATIONSERVER_TOKENEXPIREDEXCEPTION_H


#include <exception>

class TokenExpiredException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "La sesión expiró.";
    }

};

#endif //APPLICATIONSERVER_TOKENEXPIREDEXCEPTION_H
