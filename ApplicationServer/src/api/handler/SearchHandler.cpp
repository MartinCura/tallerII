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
bool SearchHandler::myfunction (Person* p1,Person* p2) {
    return p1->getTotalOfRecommendations() > p2->getTotalOfRecommendations();
}

Response *SearchHandler::handleGetRequest(http_message *httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    std::cout << queryParams <<endl;
    Response* response = new Response();
    string searchBy_type, search_value;
    long page_size, page_number;
    try {
        searchBy_type = this->getSearchByFromQueryParams(queryParams, &search_value);
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
        result = personManager->searchByName(search_value);
    } else if (searchBy_type.compare("mail") == 0) {
        result = personManager->searchByMail(search_value);
    } else if (searchBy_type.compare("skill") == 0) {
        result = personManager->searchBySkill(search_value);
    } else {
        result = personManager->searchByJobPosition(search_value);
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


bool SearchHandler::hasParameter(std::string query_params, std::string parameter) {
    size_t found = query_params.find(parameter);

    if (found == string::npos) {
        return false;
    }
    if ((found + parameter.length() + 2) > query_params.length()) {
        return false;
    }
    return true;

}

string SearchHandler::getParameterFromQueryParams(string queryParams, string parameter) {
    size_t init_position = queryParams.find(parameter);
    if (init_position == string::npos) {
        throw InvalidRequestException("Missing " + parameter + " parameter");
    }
    std::string result = "";
    //+1 porque tiene que ser la primera letra despues del =
    for (int i = (int)init_position + (int)parameter.size() + 1; i < queryParams.size(); i++) {
        if (queryParams[i] == '&' || queryParams[i] == '\n') {
            return result;
        }
        result += queryParams[i];
    }
}


string SearchHandler::getSearchByFromQueryParams(string query_params, string *search_value) {
     //elemento de busqueda
    if (hasParameter(query_params, "name")) {
        *search_value = this->getParameterFromQueryParams(query_params, "name");
        return  "name";
    }

    if (hasParameter(query_params, "position")) {
        *search_value = this->getParameterFromQueryParams(query_params, "position");
        return "position";
    }

    if (hasParameter(query_params, "skill")) {
        *search_value = this->getParameterFromQueryParams(query_params, "skill");
        return "skill";
    }

    if (hasParameter(query_params, "mail")) {
        *search_value = this->getParameterFromQueryParams(query_params, "mail");
        return "mail";
    }
    *search_value = "";
    throw InvalidRequestException("No cumple con ninguno de los posibles filtros de busqueda. Están disponibles: skill/position/mail/name");
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

SearchHandler::SearchHandler() {
    getPublic = true;
}
