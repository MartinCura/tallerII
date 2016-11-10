//
// Created by milena on 09/11/16.
//

#include <iostream>
#include <algorithm>
#include "TopTenHandler.h"
#include "../../person/PersonManager.h"

Response *TopTenHandler::handlePostRequest(http_message *httpMessage) {
    return this->getNotImplementedResponse();
}

/// Ordena de mayor a menor.
/// \param p1
/// \param p2
/// \return true si p1 es mayor a p2
bool myfunction (Person* p1,Person* p2) {
    return p1->getTotalOfRecommendations() > p2->getTotalOfRecommendations();
}

Response *TopTenHandler::handleGetRequest(http_message *httpMessage, string url) {
    string queryParams = this->getStringFromMgStr(httpMessage->query_string);
    std::cout << queryParams <<endl;
    Response* response = new Response();
    string orderBy_type, whichOne;
    try {
        orderBy_type = this->getOrderTypeFromQueryParams(queryParams);
        whichOne = this->getWhichOneFromQueryParams(queryParams);

    } catch (InvalidRequestException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
        return response;
    }

    PersonManager* personManager = new PersonManager(this->db);
    vector<Person*>* result;
    if (orderBy_type.compare("skill")) {
        result = personManager->searchBySkill(whichOne);
    } else {
        result = personManager->searchByJobPosition(whichOne);
    }

    std::sort ((*result).begin(), (*result).end(), myfunction);

    //TODO:COMPROBAR QUE EL SKILL SEA VALIDO
    Json::Value jresult;
    long condition;
    if (result->size() < 10) condition = result->size();
    else condition = 10;
    for (int i = 0; i < condition; i++) {
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

Response *TopTenHandler::handleDeleteRequest(http_message *httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response *TopTenHandler::handlePutRequest(http_message *httpMessage, string url) {
    return this->getNotImplementedResponse();
}

string TopTenHandler::getOrderTypeFromQueryParams(string query_params) {

    string orderBy_type = this->getParameterFromQueryParams(query_params, "orderBy");
    if (orderBy_type.compare("skill") != 0 || orderBy_type.compare("position") != 0) {
        throw InvalidRequestException("No cumple con ninguno de los posibles ordenes. Están disponibles: skill o position");
    }
    return orderBy_type;
}

string TopTenHandler::getWhichOneFromQueryParams(string query_params) {
    return this->getParameterFromQueryParams(query_params, "whichOne");
}


