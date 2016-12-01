#ifndef APPLICATIONSERVER_HANDLER_H
#define APPLICATIONSERVER_HANDLER_H

#include <cstring>
#include "../../Mongoose/mongoose.h"
#include "../../jsoncpp/json/json.h"
#include "../../jsoncpp/json/json-forwards.h"
#include "../Response.h"
#include "../../Exceptions/InvalidRequestException.h"
#include "../../Exceptions/InvalidTokenException.h"
#include "../../Exceptions/TokenExpiredException.h"
#include "../../session/Session.h"
#include "../../DB/DBWrapper.h"
#include "../../config/Config.h"

using namespace std;

class Handler {
public:
    Handler();
    virtual ~Handler();

    Response* handleRequest(http_message* httpMessage, string url);
    virtual Response* handlePostRequest(http_message* httpMessage) = 0;
    virtual Response* handleGetRequest(http_message* httpMessage, string url) = 0;
    virtual Response* handleDeleteRequest(http_message* httpMessage, string url) = 0;
    virtual Response* handlePutRequest(http_message* httpMessage, string url) = 0;

protected:
    Response* getNotImplementedResponse();
    Json::Value parseBody(string body);
    vector<string> parseUrl(string url);
    long getUserIdFromUrl(string url);

    bool putPublic = false;
    bool getPublic = false;
    bool postPublic = false;
    bool deletePublic = false;

    Session* session = nullptr;
    DBWrapper* db = nullptr;

    string getStringFromMgStr(const struct mg_str uri);
    string getParameterFromQueryParams(string queryParams, string parameter);

private:
    int isEqual(const struct mg_str *s1, const struct mg_str *s2);
    string getSubUrl(string url);

    /*
     * Returns as a String the value of the Header sent as a parameter <name>
     * If not found returns ""
     */
    string getHttpHeader(http_message *message, string name);

    Session * controlAccess(http_message *httpMessage);

    string *namedb;
};

#endif //APPLICATIONSERVER_HANDLER_H
