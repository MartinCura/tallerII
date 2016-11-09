//
// Created by milena on 09/11/16.
//

#ifndef APPLICATIONSERVER_SEARCHHANDLER_H
#define APPLICATIONSERVER_SEARCHHANDLER_H


#include "Handler.h"

class SearchHandler : public Handler{
public:
    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
private:

    string getOrderTypeFromQueryParams(string query_params);

    string getWhichOneFromQueryParams(string query_params);
};


#endif //APPLICATIONSERVER_SEARCHHANDLER_H
