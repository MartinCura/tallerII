//
// Exception that is thrown when de user enters an invalid password.
//

#ifndef APPLICATIONSERVER_INVALIDPASSWORDEXCEPTION_H
#define APPLICATIONSERVER_INVALIDPASSWORDEXCEPTION_H


#include <exception>

class InvalidPasswordException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Combinación mail de usuario y contraseña inválida.";
    }

};

#endif //APPLICATIONSERVER_INVALIDPASSWORDEXCEPTION_H
