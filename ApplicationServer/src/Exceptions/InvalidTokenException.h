//
// Created by milena on 02/10/16.
//

#ifndef APPLICATIONSERVER_INVALIDTOKENEXCEPTION_H
#define APPLICATIONSERVER_INVALIDTOKENEXCEPTION_H

#include <exception>

class InvalidTokenException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "El login no existe en el sistema";
    }

};


#endif //APPLICATIONSERVER_INVALIDTOKENEXCEPTION_H
