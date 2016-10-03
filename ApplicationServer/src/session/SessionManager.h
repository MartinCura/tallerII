//
// Created by milena on 02/10/16.
//

#ifndef APPLICATIONSERVER_SESSIONMANAGER_H
#define APPLICATIONSERVER_SESSIONMANAGER_H

#include <string>
#include "../DB/DBWrapper.h"

class SessionManager {
public:
    SessionManager(std::string nameDB);
    ~SessionManager();
    std::string createSessionToken();
    std::string getNewToken(std::string user_mail);
    std::string checkSession(std::string token);
    void destroyDB();

private:
    DBWrapper* db;
    std::string nameDB;

};


#endif //APPLICATIONSERVER_SESSIONMANAGER_H
