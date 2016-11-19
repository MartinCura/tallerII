#include "LoginWithFacebookHandler.h"

const string LoginWithFacebookHandler::TOKEN_KEY = "token";
const string LoginWithFacebookHandler::FIRST_NAME = "first_name";
const string LoginWithFacebookHandler::LAST_NAME = "last_name";
const string LoginWithFacebookHandler::BIRTHDAY= "birthday";
const string LoginWithFacebookHandler::EMAIL = "email";

LoginWithFacebookHandler::LoginWithFacebookHandler() {
    postPublic = true;
}

LoginWithFacebookHandler::~LoginWithFacebookHandler() {}

Response* LoginWithFacebookHandler::handleGetRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* LoginWithFacebookHandler::handlePostRequest(http_message* httpMessage) {
    string requestBody = string(httpMessage->body.p);
    Response* response = new Response();
    PersonManager *personManager = new PersonManager(this->db);
    Person* person = new Person();
    SessionManager* sessionManager = new SessionManager(this->db);
    try {
        Json::Value parsedBody = this->parseBody(requestBody);
        Json::Value data = this->getUserDataFromFacebook(parsedBody);
        person = this->createPerson(data);
        long personId;
        if (!personManager->userExists(person->getEmail())) {
            personId = personManager->savePerson(0, person->serializeMe());
        } else {
            string email = person->getEmail();
            person = personManager->getPersonByMail(&email);
            personId = person->getId();
        }
        string user_token = sessionManager->login(person->getEmail(), "");
        Json::Value responseAsJson;
        responseAsJson[LoginHandler::TOKEN_KEY] = user_token;
        responseAsJson[LoginHandler::USER_ID_KEY] = personId;
        response->setBody(responseAsJson.toStyledString());
        response->setSuccessfulHeader();
    } catch (InvalidRequestException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    } catch (ErrorLoginWithFacebookException &e) {
        response->setBadRequestHeader();
        response->setErrorBody(e.getMessage());
    }
    delete sessionManager;
    delete person;
    delete personManager;
    return response;
}

Response* LoginWithFacebookHandler::handleDeleteRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Response* LoginWithFacebookHandler::handlePutRequest(http_message* httpMessage, string url) {
    return this->getNotImplementedResponse();
}

Json::Value LoginWithFacebookHandler::getUserDataFromFacebook(Json::Value requestAsJson) {
    if (!requestAsJson.isMember(TOKEN_KEY)) throw InvalidRequestException("Missing token");
    string token = requestAsJson[TOKEN_KEY].asString();

    string request = this->buildStringRequest(token);
    string response = this->performRequest(request);
    Logger::getInstance()->debug("Request realizado: \n" + request + "\n");
    Logger::getInstance()->debug("Respuesta: \n" + response + "\n");

    Json::Value parsedResponse = this->parseResponse(response);
    Logger::getInstance()->debug("Respuesta parseada: \n" + parsedResponse.toStyledString() + "\n");
    if (parsedResponse.isMember("error")) {
        throw ErrorLoginWithFacebookException(parsedResponse["error"]["message"].asString());
    }
    return parsedResponse;
}

Person* LoginWithFacebookHandler::createPerson(Json::Value data) {
    string firstName = data[FIRST_NAME].asString();
    string lastName = data[LAST_NAME].asString();
    string birthday = data[BIRTHDAY].asString();
    string email = data[EMAIL].asString();
    Person* person = new Person();
    person->setFirstName(firstName);
    person->setLastName(lastName);
    person->setDateOfBirth(birthday);
    person->setEmail(email);
    person->setPassword("");
    return person;
}

string LoginWithFacebookHandler::buildStringRequest(string tokenToSend) {
    string getCommand = "curl";
    string url = "https://graph.facebook.com/v2.8/me";
    string fields = "fields=" + FIRST_NAME + "," + LAST_NAME + "," + BIRTHDAY + "," + EMAIL;
    string token = "access_token=" + tokenToSend;
    string request = getCommand + " " + url + "?" + fields + "&" + token;
    return request;
}

string LoginWithFacebookHandler::performRequest(string request) {
    try {
        char buffer[128];
        std::string result = "";
        std::shared_ptr<FILE> pipe(popen(request.c_str(), "r"), pclose);
        if (!pipe) throw std::runtime_error("Failed request to Facebook");
        while (!feof(pipe.get())) {
            if (fgets(buffer, 128, pipe.get()) != NULL)
                result += buffer;
        }
        return result;
    } catch (exception &e) {
        throw ErrorLoginWithFacebookException("Failed request to Facebook");
    }
}

Json::Value LoginWithFacebookHandler::parseResponse(string response) {
    size_t positionCurlyBraceOpen = response.find_first_of('{', 0);
    size_t positionCurlyBraceClose = response.find_first_of('}', 0);
    string subResponse(response.begin() + positionCurlyBraceOpen, response.begin() + positionCurlyBraceClose);
    int sections = this->getResponseSections(subResponse);
    for (int i = 0; i < sections; i++) {
        size_t positionCurlyBraceClose = subResponse.find_first_of('}', positionCurlyBraceClose);
        string subResponse2(subResponse.begin() + positionCurlyBraceClose + 1, subResponse.begin() + subResponse.size());
        subResponse = subResponse2;
    }
    string subResponse3(response.begin() + positionCurlyBraceOpen, response.begin() + positionCurlyBraceClose + 2);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(subResponse3, root);
    if (!parsingSuccessful) {
        throw ErrorLoginWithFacebookException("Error parsing Facebook response");
    }
    return root;
}

int LoginWithFacebookHandler::getResponseSections(string subResponse) {
    int sections = 0;
    int maxIterations = 20;
    string subSection = subResponse;
    size_t found = subSection.find_first_of("{");
    while (found != string::npos && sections < maxIterations) {
        string subUrl(subSection.begin() + found + 1, subSection.begin() + subSection.size());
        subSection = subUrl;
        found = subSection.find_first_of("{");
        sections++;
    }
    return sections;
}
