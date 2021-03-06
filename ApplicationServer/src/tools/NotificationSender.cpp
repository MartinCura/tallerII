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

string NotificationSender::buildContactRequest(long fromUserId, long toUserId, string name, string token) {
    string data = "{\\\"solicitud\\\":{";
    data += "\\\"from\\\":" + to_string(fromUserId) + ",";
    data += "\\\"fromNombre\\\":\\\"" + name + "\\\",";
    data += "\\\"toId\\\":" + to_string(toUserId);
    data += "}}";
    string request = this->buildCommonRequest(data, token);
    return request;
}

string NotificationSender::buildCommonRequest(string data, string token) {
    string postCommand = "curl -X POST";
    string header = "";
    header += "--header \"Authorization: key=" + Config::getInstance()->get(Config::FIREBASE_AUTHORIZATION_KEY) + "\"";
    header += " ";
    header += "--header \"Content-Type: application/json\"";
    string url = Config::getInstance()->get(Config::FIREBASE_URL);
    string option = "-d";
    string body = "";
    body += "\"{\\\"to\\\":\\\"" + token + "\\\",\\\"data\\\":" + data  + "}\"";
    string request = postCommand + " " + header + " " + url + " " + option + " " + body;
    return request;
}

string NotificationSender::performRequest(string request) {
    if (Config::getInstance()->get(Config::DUMMY_SERVER_ENABLED) == "true") {
        DummyServer* dummyServer = new DummyServer();
        string result = dummyServer->getFirebaseDummyResponse();
        delete dummyServer;
        return result;
    }
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
