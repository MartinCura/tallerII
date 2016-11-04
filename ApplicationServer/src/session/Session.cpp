//
// Created by milena on 03/11/16.
//

#include "Session.h"

void Session::setToken(std::string token) {
    this->token = token;
}

void Session::setUserMail(std::string user_mail) {
    this->user_mail = user_mail;
}

void Session::setLastTime(time_t last_time) {
    this->last_time = last_time;
}

void Session::setUserID(long i) {
    this->user_id = i;
}

long Session::getUserId() {
    return this->user_id;
}

std::string Session::getUserMail() {
    return this->user_mail;
}

time_t Session::getLastTime() {
    return this->last_time;
}
