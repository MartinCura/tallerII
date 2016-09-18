#include "UsersHandler.h"

UsersHandler::UsersHandler() {}

UsersHandler::~UsersHandler() {}

Response* UsersHandler::handlePostRequest(http_message* httpMessage) {
    string body = string(httpMessage->body.p);
    try {
        Json::Value parsedBody = this->parseBody(body);
        string name = parsedBody.get("name", "").asString();
        if (name == "") {
            return this->getBadRequestResponse("Missing name parameter.");
        }
        Response* response = new Response();
        response->setSuccessfulHeader();

        //FIXME: reemplazar por info de la base
        Json::Value root;
        root["id"] = 1;

        string responseBody = root.toStyledString();
        response->setBody(responseBody.c_str());

        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    }
}

Response* UsersHandler::handleGetRequest(http_message* httpMessage, string url) {
    try {
        Response* response = new Response();
        response->setSuccessfulHeader();
        string responseBody = buildResponse(this->getUserId(url));
        response->setBody(responseBody.c_str());
        return response;
    } catch (InvalidRequestException& e) {
        return this->getBadRequestResponse(e.getMessage());
    } catch (UserNotFoundException& e) {
        return this->getNotFoundResponse(e.getMessage());
    }
}

Response* UsersHandler::handleDeleteRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

Response* UsersHandler::handlePutRequest(http_message* httpMessage) {
    return this->getNotImplementedResponse();
}

int UsersHandler::getUserId(string url) {
    size_t sp = url.find_first_of('/', 1);
    if (sp == string::npos || ((url.begin() + sp + 1) >= (url.begin() + url.size()))) {
        throw InvalidRequestException("Cannot get user id from url.");
    }
    string userIdAsString(url.begin() + sp + 1, url.begin() + url.size());
    try {
        int userId = stoi(userIdAsString);
        return userId;
    } catch (invalid_argument e) {
        throw InvalidRequestException("Not a numeric id");
    }
}

string UsersHandler::buildResponse(int id) {
    PersonManager *personManager = new PersonManager();
    Person *person = personManager->getPersonById(id);
    Json::Value response;
    response["id"] = id;
    response["first_name"] = person->getFirstName();
    response["last_name"] = person->getLastName();
    response["email"] = person->getEmail();
    response["date_of_birth"] = person->getDateOfBirth();
    response["city"] = person->getCity();
    response["profile_picture"] = person->getProfilePicture();
    response["summary"] = person->getSummary();
    vector<WorkHistory*> workHistoryVector = person->getWorkHistory();
    for (vector<WorkHistory*>::size_type i = 0; i != workHistoryVector.size(); i++) {
        Json::Value workHistoryResponse;
        WorkHistory* workHistory = workHistoryVector[i];
        workHistoryResponse["positionTitle"] = workHistory->getPositionTitle();
        workHistoryResponse["company"] = workHistory->getCompany();
        workHistoryResponse["fromDate"] = workHistory->getFromDate();
        workHistoryResponse["toDate"] = workHistory->getToDate();
        response["work_history"].append(workHistoryResponse);
    }
    vector<Skill*> skillsVector = person->getSkills();
    for (vector<Skill*>::size_type i = 0; i != skillsVector.size(); i++) {
        Json::Value skillsResponse;
        Skill* skill = skillsVector[i];
        skillsResponse["name"] = skill->getName();
        skillsResponse["description"] = skill->getDescription();
        skillsResponse["category"] = skill->getCategory();
        response["skills"].append(skillsResponse);
    }
    delete person;
    delete personManager;
    return response.toStyledString();
}
