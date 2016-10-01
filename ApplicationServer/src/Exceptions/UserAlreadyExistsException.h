//
// Excepción que se lanza cuando un usuario ya existe en la base de datos.
//

#ifndef APPLICATIONSERVER_USERALREADYEXISTSEXCEPTION_H
#define APPLICATIONSERVER_USERALREADYEXISTSEXCEPTION_H

#include <exception>

class UserAlreadyExistsException: public std::exception {
public:
    virtual const char* what() const throw() {
        return "El usuario ya existe en el sistema";
    }

};


#endif //APPLICATIONSERVER_USERALREADYEXISTSEXCEPTION_H
