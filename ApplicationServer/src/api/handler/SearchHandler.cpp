//
// Created by milena on 09/11/16.
//

#include <iostream>
#include <algorithm>
#include "SearchHandler.h"
#include "../../person/PersonManager.h"


const char HEX2DEC[256] =
        {
                /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
                /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

                /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

                /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

                /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
        };

std::string uriDecode(const std::string & sSrc)
{
    // Note from RFC1630:  "Sequences which start with a percent sign
    // but are not followed by two hexadecimal characters (0-9, A-F) are reserved
    // for future extension"

    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const int SRC_LEN = sSrc.length();
    const unsigned char * const SRC_END = pSrc + SRC_LEN;
    const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%'

    char * const pStart = new char[SRC_LEN];
    char * pEnd = pStart;

    while (pSrc < SRC_LAST_DEC)
    {
        if (*pSrc == '%')
        {
            char dec1, dec2;
            if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
            {
                *pEnd++ = (dec1 << 4) + dec2;
                pSrc += 3;
                continue;
            }
        }

        *pEnd++ = *pSrc++;
    }

    // the last 2- chars
    while (pSrc < SRC_END)
        *pEnd++ = *pSrc++;

    std::string sResult(pStart, pEnd);
    delete [] pStart;
    return sResult;
}

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


void SearchHandler::split2(const std::string &s, char delim, vector<string> *elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems->push_back(item);
    }
}

vector<string>* SearchHandler::split(const std::string &s, char delim) {
    std::vector<std::string>* elems = new std::vector<std::string>();
    split2(s, delim, elems);
    return elems;
}


Response *SearchHandler::handleGetRequest(http_message *httpMessage, string url) {

    string uri = this->getStringFromMgStr(httpMessage->query_string);
    std::string queryParams = uriDecode(uri);

    Response* response = new Response();
    long page_size, page_number;
    std::string *search_value_name, *search_value_mail, *search_value_skill, *search_value_distance, *search_value_position;
    search_value_name = nullptr;
    search_value_distance = nullptr;
    search_value_position = nullptr;
    search_value_skill = nullptr;
    search_value_distance = nullptr;
    try {
        search_value_name = this->getSearchByFromQueryParams(queryParams, "names");
        search_value_distance = this->getSearchByFromQueryParams(queryParams, "distances");
        search_value_position = this->getSearchByFromQueryParams(queryParams, "positions");
        search_value_skill = this->getSearchByFromQueryParams(queryParams, "skills");
        search_value_mail = this->getSearchByFromQueryParams(queryParams, "mails");
        page_number = this->getPageNumberFromQueryParams(queryParams);
        page_size = this->getPageSizeFromQueryParams(queryParams);

    } catch (InvalidRequestException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
        return response;
    }

    //search_value_ sera un string con todos los valores que quieran encontrarse en la persona a hallar
    //separados todos ellos por coma
    //Ejemplo: skill=skill1,skill2,skill3

    //Parseo por ,

    std::vector<string>* skills = nullptr, *mails = nullptr, *names = nullptr, *positions = nullptr, *distance = nullptr;
    if (search_value_skill != nullptr) skills = this->split(*search_value_skill, ',');
    if (search_value_mail != nullptr) mails = this->split(*search_value_mail, ',');
    if (search_value_name != nullptr) names = this->split(*search_value_name, ',');
    if (search_value_position != nullptr) positions = this->split(*search_value_position, ',');
    if (search_value_distance != nullptr) distance = this->split(*search_value_distance, ',');


    PersonManager* personManager = new PersonManager(this->db);
    vector<Person*>* result;
    std::map<string, vector<string>*>* search_values = new std::map<string, vector<string>*>();


    if (search_value_distance == nullptr && search_value_skill == nullptr &&
        search_value_name == nullptr && search_value_mail==nullptr &&
        search_value_position == nullptr) {
        result = personManager->getAllUsers();
    }else {

        search_values->insert(std::pair<string, vector<string>*>("name",names));
        search_values->insert(std::pair<string, vector<string>*>("positions",positions));
        search_values->insert(std::pair<string, vector<string>*>("skill",skills));
        search_values->insert(std::pair<string, vector<string>*>("distance",distance));
        search_values->insert(std::pair<string, vector<string>*>("mail",mails));

        result = personManager->search_users_by(search_values);
    }

    delete search_value_distance;
    delete search_value_mail;
    delete search_value_name;
    delete search_value_position;
    delete search_value_skill;



    std::sort ((*result).begin(), (*result).end(), myfunction);
    //FIXME: FALTA DETERMINAR COMO SE PUEDE ORDENAR Y HACERLO
    //FIxme: ver como martin&martin pueden saber la cantidad de paginas que me pueden pedir.
    long start = (page_number - 1)*page_size;
    long end;
    if ((start + page_size) > result->size()) end = result->size();
    else end = start + page_size;



    Json::Value jresult;
    jresult["metadata"]["total_count"] = result->size();
    for (long i = start; i< end; i++) {
        Json::Value user_minInformation;
        user_minInformation["first_name"] = (*result)[i]->getFirstName();
        user_minInformation["last_name"] = (*result)[i]->getLastName();
        user_minInformation["id"] = (*result)[i]->getId();
        user_minInformation["current_job"] = ((*result)[i]->getCurrentJob() == nullptr) ? "":(*result)[i]->getCurrentJob()->serializeMe();
        user_minInformation["tot_recommendations"] = (*result)[i]->getTotalOfRecommendations();
        user_minInformation["mail"] = (*result)[i]->getEmail();
        //user_minInformation["picture]
        jresult["result"].append(user_minInformation);
    }

    response->setBody(jresult.toStyledString());
    response->setSuccessfulHeader();


    //delete de todas los users guardados en el vector result.
    for (std::vector<Person*>::iterator iterator = result->begin(); iterator != result->end(); ++iterator) {
        delete (*iterator);
    }

    //delete de todos los vectores que guardan los argumentos de busqueda en el map.
    for (std::map<string, vector<string>*>::iterator iter = search_values->begin(); iter != search_values->end(); ++iter) {
        delete (iter->second);
    }
    delete search_values;
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
        if (queryParams[i] == '&' || queryParams[i] == '\0') {
            return result;
        }
        result += queryParams[i];
    }
    return result;
}


string * SearchHandler::getSearchByFromQueryParams(string query_params, std::string parameter) {
     //elemento de busqueda
    if (hasParameter(query_params, parameter)) {
        return new string(this->getParameterFromQueryParams(query_params, parameter));
    }
    return nullptr;
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
