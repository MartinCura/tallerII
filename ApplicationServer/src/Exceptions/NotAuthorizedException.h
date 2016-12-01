//
// Created by milena on 03/11/16.
//

#ifndef APPLICATIONSERVER_NOTAUTHORIZEDEXCEPTION_H
#define APPLICATIONSERVER_NOTAUTHORIZEDEXCEPTION_H


#include <exception>

class NotAuthorizedException: public std::exception {

    public:
        virtual const char* what() const throw() {
            return "El usuario no tiene permisos para realizar la acción";
        }

};


#endif //APPLICATIONSERVER_NOTAUTHORIZEDEXCEPTION_H
