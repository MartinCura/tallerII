//
// Clase dedicada a manejar las acciones asociadas a la sesión de usuario
// en la aplicación.
//

#ifndef APPLICATIONSERVER_SESSIONMANAGER_H
#define APPLICATIONSERVER_SESSIONMANAGER_H

#include <string>
#include "../DB/DBWrapper.h"
#include "Session.h"

#define USER_TOKEN "user:token_"
#define USER_MAIL_ID "user:mail_"
#define USER_PASSWORD "user:password_"

class SessionManager {
public:
    SessionManager(DBWrapper *db);
    ~SessionManager();



    /*
     * Comprueba que el <token> de sesión corresponda
     * con una sesión abierta y válida.
     * Válida implica que el token existe y el tiempo de inactividad
     * del usuario no es mayor a 4 m.
     * Actualiza la ultima vez que el token fue usado siempre y cuando sea valido.
     */
    Session * getSession(std::string token);
    /*
     * Comprueba a partir de <user_mail> y <user_password>
     * corresponden a un usuario en el sistema, y si la combinación es correcta.
     * En el ultimo caso, devuelve el token correspondiente a la sesión.
     */
    std::string login(std::string user_mail, std::string user_password);
    /*
     * Devuelve el id del usuario que corresponde con el <token>.
     * Se supone que el token corresponde a un token válido.
     */
    long getUserId(std::string token);
    std::string facebookLogin(std::string user_mail);

private:
    DBWrapper* db;

    bool tokenExpired(std::string last_time_used);
    /*
     * Guarda la información necesaria en la Base de Datos.
     * La "información necesaria" corresponde con el siguiente formato:
     * USER_TOKEN + user_mail = { "user_token": <token> , "last_used":<cuando fue usado por ultima vez>}
     * USER_TOKEN + user_token = { "user_mail": <user_mail>, "last_used":<cuando fue usado por ultima vez>}
     * El primer formato tiene como objetivo encontrar el token a partir del mail.
     * El segundo formato tiene como objetivo encontrar la informacion a partir del token.
     */
    void saveToken(std::string token, std::string user_mail);
    /*
     * Genera el codigo del token.
     */
    std::string getNewToken();
};


#endif //APPLICATIONSERVER_SESSIONMANAGER_H
