#ifndef APPLICATIONSERVER_WEBHANDLER_H
#define APPLICATIONSERVER_WEBHANDLER_H

#include "../Mongoose/mongoose.h"
#include "Response.h"
#include "handler/Handler.h"
#include "handler/UsersHandler.h"
#include "handler/PictureHandler.h"
#include "handler/ContactsHandler.h"
#include "handler/LoginHandler.h"
#include "handler/RecommendationsHandler.h"
#include "handler/MessagesHandler.h"
#include "handler/NotificationTokenHandler.h"
#include "../logger/Logger.h"
#include <regex>
#include <cstring>

using namespace std;

class WebHandler {
public:
    WebHandler();
    ~WebHandler();

    Response* handleRequest(http_message* httpMessage);

private:
    string getUrl(const struct mg_str uri);
    string getStringFromMgStr(const struct mg_str uri);
    void logRequest(http_message* httpMessage, bool logBody = true);
    void logResponse(Response* response, bool logBody = true);
};


#endif //APPLICATIONSERVER_WEBHANDLER_H
