//
// Created by milena on 03/11/16.
//

#ifndef APPLICATIONSERVER_SESSION_H
#define APPLICATIONSERVER_SESSION_H

#include <string>

class Session {

public:
    Session(){};
    ~Session(){};

    void setToken(std::string token);
    void setUserMail(std::string user_mail);
    void setLastTime(time_t last_time);
    void setUserID(long i);

    long getUserId();
    std::string getUserMail();
    time_t getLastTime();

private:
    std::string token;
    std::string user_mail;
    time_t last_time;
    long user_id;



};


#endif //APPLICATIONSERVER_SESSION_H
