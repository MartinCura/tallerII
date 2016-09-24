#ifndef APPLICATIONSERVER_WEBHANDLER_H
#define APPLICATIONSERVER_WEBHANDLER_H

#include "../Mongoose/mongoose.h"
#include "Response.h"
#include "handler/Handler.h"
#include "handler/UsersHandler.h"
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
};


#endif //APPLICATIONSERVER_WEBHANDLER_H
