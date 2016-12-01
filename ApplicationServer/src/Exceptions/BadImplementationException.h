//
// Created by milena on 09/11/16.
//

#ifndef APPLICATIONSERVER_BADIMPLEMENTATIONEXCEPTION_H
#define APPLICATIONSERVER_BADIMPLEMENTATIONEXCEPTION_H


#include <exception>

class BadImplementationException: public std::exception {

public:
    virtual const char* what() const throw() {
        return "Implementación mal hecha. Debugear.";
    }
};


#endif //APPLICATIONSERVER_BADIMPLEMENTATIONEXCEPTION_H
