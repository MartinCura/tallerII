//
// Created by milena on 02/10/16.
//

#ifndef APPLICATIONSERVER_INVALIDTOKENEXCEPTION_H
#define APPLICATIONSERVER_INVALIDTOKENEXCEPTION_H

#include <exception>

class InvalidTokenException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "El login no corresponde con una sesión abierta valida   ";
    }

};


#endif //APPLICATIONSERVER_INVALIDTOKENEXCEPTION_H
