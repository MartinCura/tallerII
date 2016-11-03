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


std::string SessionManager::createSessionToken() {
    int token_length;

    token_length = 8;
    char token[8];

    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    srand ((unsigned int)time(NULL));

    for (int i = 0; i < token_length; ++i) {
        token[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    token[token_length] = '\0';
    return  std::string(token);
}


SessionManager::SessionManager(std::string nameDB) {
    db = new DBWrapper();
    this->nameDB = nameDB;

    DBWrapper::ResponseCode status = db->openDb(nameDB);
    if (status == DBWrapper::ResponseCode::ERROR) {
        throw std::exception();
    }
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

    return this->getNewToken(user_mail);
    //TODO: GUARDAR CONEXIÓN ABIERTA

}


SessionManager::~SessionManager() {
    if (db != nullptr) {
        db->deleteDB();
    }
}

///Pre: Se pre-supone que el user_mail pasado por parámetro corresponde al de un usuario valido.
std::string SessionManager::getNewToken(std::string user_mail) {
    std::string new_token, token_string, token2_string;
    time_t creation_time;
    Json::Value token, token2;
    Json::FastWriter fastWriter;
    char buff[20];

    //Se obtiene el tiempo actual.
    creation_time = time(NULL);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&creation_time));
    std::string now  = std::string(buff);

    new_token = createSessionToken();

    token["user_token"] = new_token;
    token["last_used"] = now;

    token_string = fastWriter.write(token);

    token2["user_mail"] = user_mail;
    token2["last_used"] = now;

    token2_string = fastWriter.write(token2);

    db->puTKey(USER_TOKEN + user_mail, &token_string);
    db->puTKey(USER_TOKEN + new_token, &token2_string);

    return new_token;
}

long SessionManager::getUserId(std::string token) {
    std::string token_information, user_information;
    Json::Reader reader;
    Json::Value token_information_json, user_information_json;

    db->getKey(USER_TOKEN + token, &token_information);

    reader.parse(token_information.c_str(), token_information_json);

    //TODO: CAMBIAR LA FORMA DE ACCEDER A LA INFORMACIÓN DEL USUARIO MEDIANTE ID.
    db->getKey(USER_MAIL_ID + token_information_json["user_mail"].asString(), &user_information);

    reader.parse(user_information.c_str(), user_information_json);


    return user_information_json["id"].asLargestInt();

}

Session * SessionManager::checkSession(std::string token) {

    std::string token_information, last_time_used;
    Json::Reader reader;
    Json::Value json_token_information;


    if (!db->existsKey(USER_TOKEN + token, &token_information))throw  InvalidTokenException();

    reader.parse( token_information.c_str(), json_token_information );
    last_time_used = json_token_information["last_used"].asString();

    if (tokenExpired(last_time_used)) throw TokenExpiredException();

    Session* session = new Session();
    session->setLastTime(last_time_used);
    session->setUserMail(json_token_information["user_mail"].asString());
    session->setToken(token);
    session->setUserID(this->getUserId(token)); //todo: cambiar la forma de obtener el id desde session.

    return session;

}

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

void SessionManager::destroyDB() {
    db->deleteDB();
    db->destroyDB(this->nameDB);

}

