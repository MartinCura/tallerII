//
// Created by milena on 09/11/16.
//

#ifndef APPLICATIONSERVER_SEARCHHANDLER_H
#define APPLICATIONSERVER_SEARCHHANDLER_H


#include "Handler.h"
#include "../../person/Person.h"

class SearchHandler: public Handler {
public:
    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
private:

    string getSearchByFromQueryParams(string query_params);
    string getWhichOneFromQueryParams(string query_params);
    long getPageSizeFromQueryParams(string query_params);
    long getPageNumberFromQueryParams(string query_params);
    bool static myfunction(Person *p1, Person *p2);
};


#endif //APPLICATIONSERVER_SEARCHHANDLER_H
