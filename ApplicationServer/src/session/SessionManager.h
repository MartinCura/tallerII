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
    SessionManager(std::string nameDB);
    ~SessionManager();

    /*
     * Obtiene un token nuevo para la sesión y guarda la información necesaria en la Base de Datos.
     * La "información necesaria" corresponde con el siguiente formato:
     * USER_TOKEN + user_mail = { "user_token": <token> , "last_used":<cuando fue usado por ultima vez>}
     * USER_TOKEN + user_token = { "user_mail": <user_mail>, "last_used":<cuando fue usado por ultima vez>}
     * El primer formato tiene como objetivo encontrar el token a partir del mail.
     * El segundo formato tiene como objetivo encontrar la informacion a partir del token.
     */
    std::string getNewToken();
    /*
     * Comprueba que el <token> de sesión corresponda
     * con una sesión abierta y válida.
     * Válida implica que el token existe y el tiempo de inactividad
     * del usuario no es mayor a 4 m.
     */
    Session * getSession(std::string token);
    /*
     * Comprueba a partir de <user_mail> y <user_password>
     * corresponden a un usuario en el sistema, y si la combinación es correcta.
     */
    std::string login(std::string user_mail, std::string user_password);
    /*
     * Devuelve el id del usuario que corresponde con el <token>.
     * Se supone que el token corresponde a un token válido.
     */
    long getUserId(std::string token);
    /*
     * Cierra la conexión de la base de datos.
     */

    void destroyDB();

private:
    DBWrapper* db;
    std::string nameDB;


    bool tokenExpired(std::string last_time_used);

    void saveToken(std::string token, std::string user_mail);
};


#endif //APPLICATIONSERVER_SESSIONMANAGER_H
