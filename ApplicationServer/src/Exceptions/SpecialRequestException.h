//
// ???
//

#ifndef APPLICATIONSERVER_SPECIALREQUESTEXCEPTION_H
#define APPLICATIONSERVER_SPECIALREQUESTEXCEPTION_H


#include <exception>

class SpecialRequestException: public std::exception {

public:
    virtual const char* what() const throw() {
        return "Se busca algo especial...";
    }
};


#endif //APPLICATIONSERVER_SPECIALREQUESTEXCEPTION_H
