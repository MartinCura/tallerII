//
// Created by milena on 02/10/16.
//

//#include <ctime>

#include <json/json.h>
#include "SessionManager.h"
#include "../Exceptions/InvalidTokenException.h"
#include "../Exceptions/TokenExpiredException.h"
#include "../Exceptions/UserNotFoundException.h"
#include "../Exceptions/InvalidPasswordException.h"


SessionManager::SessionManager(DBWrapper *db) {
    this->db = db;
}


string SessionManager::login(std::string user_mail, std::string user_password) {

    std::string user_bdd_pasword;

    if (!db->existsKey(USER_PASSWORD + user_mail, &user_bdd_pasword )) {
        //No existe un usuario con dicho mail en la base
        throw  UserNotFoundException(user_mail);
    }

    if (user_bdd_pasword.compare(user_password) != 0) {
        //La contraseña almacenada y la de inicio de sesión no coinciden
        throw InvalidPasswordException();
    }

    //La sesión fue aceptada.
    //Se genera el login y se lo guarda en la base.

    std::string new_token = this->getNewToken();
    this->saveToken(new_token, user_mail);
    return new_token;

}

string SessionManager::facebookLogin(std::string user_mail) {
    std::string user_bdd;
    if (!db->existsKey(USER_PASSWORD + user_mail, &user_bdd)) {
        throw UserNotFoundException(user_mail);
    }
    std::string new_token = this->getNewToken();
    this->saveToken(new_token, user_mail);
    return new_token;
}

SessionManager::~SessionManager() {}

///Pre: Se pre-supone que el user_mail pasado por parámetro corresponde al de un usuario valido.
std::string SessionManager::getNewToken() {

    int token_length;

    token_length = 8;
    char token[8];

    static const char alphanum[] =
            "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";


    for (int i = 0; i < token_length; ++i) {
        token[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    token[token_length] = '\0';
    return  std::string(token);

}

void SessionManager::saveToken(std::string token, std::string user_mail) {
    std::string token_string, token2_string;
    time_t creation_time;
    Json::Value jtoken, jtoken2;
    Json::FastWriter fastWriter;
    char buff[20];

    //Se obtiene el tiempo actual.
    creation_time = time(NULL);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&creation_time));
    std::string now  = std::string(buff);

    jtoken["user_token"] = token;
    jtoken["last_used"] = now;

    token_string = fastWriter.write(jtoken);

    Person* user = this->getUserByMail(user_mail);
    long user_id = user->getId();
    delete user;
    jtoken2["user_mail"] = user_mail;
    jtoken2["user_id"] = user_id;
    jtoken2["last_used"] = now;

    token2_string = fastWriter.write(jtoken2);

    db->putKey(USER_TOKEN + user_mail, &token_string);
    db->putKey(USER_TOKEN + token, &token2_string);


}
long SessionManager::getUserId(std::string token) {
    std::string token_information, user_information;
    Json::Reader reader;
    Json::Value token_information_json, user_information_json;

    db->getKey(USER_TOKEN + token, &token_information);

    reader.parse(token_information.c_str(), token_information_json);
    return token_information_json["user_id"].asLargestInt();

}

Session * SessionManager::getSession(std::string token) {

    std::string token_information, last_time_used;
    Json::Reader reader;
    Json::Value json_token_information;


    if (!db->existsKey(USER_TOKEN + token, &token_information))throw  InvalidTokenException();

    reader.parse( token_information.c_str(), json_token_information );
    last_time_used = json_token_information["last_used"].asString();

    if (tokenExpired(last_time_used)) throw TokenExpiredException();

    //Actualizar ultima vez que fue usado.
    //Se guarda nuevamente el token, reemplazando información previa
    this->updateToken(token, json_token_information["user_mail"].asString());

    Session* session = new Session();
    session->setLastTime(time(NULL));
    session->setUserMail(json_token_information["user_mail"].asString());
    session->setToken(token);
    session->setUserID(this->getUserId(token)); //todo: cambiar la forma de obtener el id desde session.

    return session;

}
/*
void SessionManager::updateLastTimeUsed(std::string token) {


}*/

bool SessionManager::tokenExpired(std::string last_time_used) {
    const char *time_details = last_time_used.c_str();
    struct tm tm;
    strptime(time_details, "%Y-%m-%d %H:%M:%S", &tm);
    time_t t = mktime(&tm);

    time_t now = time(NULL);

    double max_time_diff = 14400;
    double diff = difftime(now, t);

    return (diff > max_time_diff);
}

void SessionManager::updateToken(string token, string user_mail) {
    std::string token_string, token2_string;
    time_t creation_time;
    Json::Value jtoken, jtoken2;
    Json::FastWriter fastWriter;
    char buff[20];
    std::string token_information;
    long user_id;
    Json::Reader reader;
    Json::Value json_token_information;


    //Se obtiene la vieja informacion del token
    if (!db->existsKey(USER_TOKEN + token, &token_information))throw  InvalidTokenException();
    reader.parse( token_information.c_str(), json_token_information );
    user_id = json_token_information["user_id"].asLargestInt();

    //Se obtiene el tiempo actual.
    creation_time = time(NULL);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&creation_time));
    std::string now  = std::string(buff);

    jtoken["user_token"] = token;
    jtoken["last_used"] = now;

    token_string = fastWriter.write(jtoken);

     jtoken2["user_mail"] = user_mail;
    jtoken2["user_id"] = user_id;
    jtoken2["last_used"] = now;

    token2_string = fastWriter.write(jtoken2);

    db->putKey(USER_TOKEN + user_mail, &token_string);
    db->putKey(USER_TOKEN + token, &token2_string);


}



