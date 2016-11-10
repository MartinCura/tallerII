//
// Created by milena on 09/11/16.
//

#include <iostream>
#include <algorithm>
#include "SearchHandler.h"
#include "../../person/PersonManager.h"

Response *SearchHandler::handlePostRequest(http_message *httpMessage) {
        return this->getNotImplementedResponse();
}

/// Ordena de mayor a menor.
/// \param p1
/// \param p2
/// \return true si p1 es mayor a p2
bool myfunction (Person* p1,Person* p2) {
    return p1->getTotalOfRecommendations() > p2->getTotalOfRecommendations();
}

Response *SearchHandler::handleGetRequest(http_message *httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    std::cout << queryParams <<endl;
    Response* response = new Response();
    string searchBy_type, whichOne;
    long page_size, page_number;
    try {
        searchBy_type = this->getSearchByFromQueryParams(queryParams);
        whichOne = this->getWhichOneFromQueryParams(queryParams);
        page_number = this->getPageNumberFromQueryParams(queryParams);
        page_size = this->getPageSizeFromQueryParams(queryParams);

    } catch (InvalidRequestException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
        return response;
    }

    PersonManager* personManager = new PersonManager(this->db);
    vector<Person*>* result;

    if (searchBy_type.compare("name") == 0) {
        result = personManager->searchByName(whichOne);
    } else if (searchBy_type.compare("mail") == 0) {
        result = personManager->searchByMail(whichOne);
    } else if (searchBy_type.compare("skill") == 0) {
        result = personManager->searchBySkill(whichOne);
    } else {
        result = personManager->searchByJobPosition(whichOne);
    }

    std::sort ((*result).begin(), (*result).end(), myfunction);
    //FIXME: FALTA DETERMINAR COMO SE PUEDE ORDENAR Y HACERLO
    //FIxme: ver como martin&martin pueden saber la cantidad de paginas que me pueden pedir.
    long start = (page_number - 1)*page_size;
    long end;
    if ((start + page_size) > result->size()) end = result->size();
    else end = start + page_size;


    Json::Value jresult;
    jresult["metadata"]["total_results"] = result->size();
    for (long i = start; i< end; i++) {
        Json::Value user_minInformation;
        user_minInformation["first_name"] = (*result)[i]->getFirstName();
        user_minInformation["last_name"] = (*result)[i]->getLastName();
        user_minInformation["id"] = (*result)[i]->getId();
        //user_minInformation["picture]
        jresult["result"].append(user_minInformation);
    }

    response->setBody(jresult.toStyledString());
    response->setSuccessfulHeader();
    delete personManager;
    delete result;
    return response;
}

Response *SearchHandler::handleDeleteRequest(http_message *httpMessage, string url) {
        return this->getNotImplementedResponse();
}

Response *SearchHandler::handlePutRequest(http_message *httpMessage, string url) {
        return this->getNotImplementedResponse();
}

string SearchHandler::getSearchByFromQueryParams(string query_params) {
    string orderBy_type = this->getParameterFromQueryParams(query_params, "orderBy");
    if (orderBy_type.compare("name") != 0 || orderBy_type.compare("position") != 0 || orderBy_type.compare("skill") != 0 || orderBy_type.compare("mail") != 0) {
        throw InvalidRequestException("No cumple con ninguno de los posibles filtros de busqueda. Están disponibles: skill/position/mail/name");
    }
    return orderBy_type;
}

string SearchHandler::getWhichOneFromQueryParams(string query_params) {
    return std::__cxx11::string();
}
long SearchHandler::getPageSizeFromQueryParams(string query_params) {

    string size_of = this->getParameterFromQueryParams(query_params, "size");
    long isize_of;
    try {
        isize_of = stol(size_of);
    } catch (exception& exception1){
        throw InvalidRequestException("Size of Page isn't an integer");
    }
    return isize_of;


}

long SearchHandler::getPageNumberFromQueryParams(string query_params) {
    string page_number = this->getParameterFromQueryParams(query_params, "pageNumber");
    long ipage_number;
    try {
        ipage_number = stol(page_number);
    } catch (exception& exception1){
        throw InvalidRequestException("Page Number isn't an integer");
    }
    return ipage_number;
}
