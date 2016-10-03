#ifndef PROJECT_ERROROPENINGDATABASEEXCEPTION_H
#define PROJECT_ERROROPENINGDATABASEEXCEPTION_H

#include <exception>

class ErrorOpeningDatabaseException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Error al abrir la base de datos";
    }
};

#endif //PROJECT_ERROROPENINGDATABASEEXCEPTION_H
