#include "NotificationSender.h"

NotificationSender::NotificationSender() {}

NotificationSender::~NotificationSender() {}

void NotificationSender::sendNotification(string request) {
    string response = this->performRequest(request);
    Logger::getInstance()->debug("Request realizado: \n" + request + "\n");
    Logger::getInstance()->debug("Respuesta: \n" + response + "\n");
}

string NotificationSender::buildMessageRequest(long fromUserId, long toUserId, string message, string timestamp, string token) {
    string data = "{\\\"mensaje\\\":{";
    data += "\\\"from\\\":" + to_string(fromUserId) + ",";
    data += "\\\"message\\\":\\\"" + message + "\\\",";
    data += "\\\"timestamp\\\":\\\"" + timestamp + "\\\",";
    data += "\\\"to\\\":" + to_string(toUserId);
    data += "}}";
    string request = this->buildCommonRequest(data, token);
    return request;
}

string NotificationSender::buildCommonRequest(string data, string token) {
    string postCommand = "curl -X POST";
    string header = "";
    header += "--header \"Authorization: key=AIzaSyD3T2nk8nqIRSN1VlPZ3QkUcrzHTD7JIfA\"";
    header += " ";
    header += "--header \"Content-Type: application/json\"";
    string url = "http://requestb.in/wilwsbwi";
    string option = "-d";
    string body = "";
    body += "\"{\\\"to\\\":\\\"" + token + "\\\",\\\"data\\\":" + data  + "}\"";
    string request = postCommand + " " + header + " " + url + " " + option + " " + body;
    return request;
}

string NotificationSender::performRequest(string request) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(request.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Failed request to Firebase");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
