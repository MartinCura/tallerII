#ifndef APPLICATIONSERVER_WEBHANDLER_H
#define APPLICATIONSERVER_WEBHANDLER_H

#include <array>
#include "../mongoose/mongoose.h"
#include "Response.h"
#include "Handler.h"
#include "UsersHandler.h"

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
