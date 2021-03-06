//
// Created by milena on 09/11/16.
//

#ifndef APPLICATIONSERVER_SEARCHHANDLER_H
#define APPLICATIONSERVER_SEARCHHANDLER_H


#include "Handler.h"
#include "../../person/Person.h"

class SearchHandler: public Handler {
public:
    SearchHandler();
    Response* handlePostRequest(http_message* httpMessage);
    Response* handleGetRequest(http_message* httpMessage, string url);
    Response* handleDeleteRequest(http_message* httpMessage, string url);
    Response* handlePutRequest(http_message* httpMessage, string url);
private:

    string * getSearchByFromQueryParams(string query_params, std::string parameter);
    long getPageSizeFromQueryParams(string query_params);
    long getPageNumberFromQueryParams(string query_params);
    bool static myfunction(Person *p1, Person *p2);
    bool hasParameter(string query_params, string parameter);
    void split2(const string &s, char delim, vector<string> *elems);
    vector<string> * split(const string &s, char delim);

    string getParameterFromQueryParams(string queryParams, string parameter);
};


#endif //APPLICATIONSERVER_SEARCHHANDLER_H
