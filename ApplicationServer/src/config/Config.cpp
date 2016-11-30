#include "Config.h"

const char *Config::NAME_DB = "DbName";
const char *Config::IMG_FOLDER = "imgDir";
const char *Config::LOGS_FOLDER = "logsDir";
const char *Config::FACEBOOK_URL = "facebookUrl";
const char *Config::FIREBASE_URL = "firebaseUrl";
const char *Config::FIREBASE_AUTHORIZATION_KEY = "firebaseAuthorizationKey";
const char *Config::DUMMY_SERVER_ENABLED = "dummyServerEnabled";

Config* Config::instance = NULL;

Config::Config() {}

Config::~Config() {}

Config* Config::getInstance() {
    if (!instance) {
        instance = new Config;
    }
    return instance;
}

void Config::load(string path) {
    string fileContent = this->loadFile(path);
    this->configValues = this->parseFile(fileContent);
}

string Config::get(string key) {
    if (!this->configValues.isMember(key)) {
        throw MissingConfigurationException(key);
    }
    return this->configValues[key].asString();
}

string Config::loadFile(string path) {
    string line;
    string fileContent;
    ifstream configFile(path.c_str());
    if (!configFile.is_open()) {
        throw "Invalid or nonexistent config file";
    }
    while (getline(configFile, line)) {
        fileContent += line;
    }
    configFile.close();
    return fileContent;
}

Json::Value Config::parseFile(string fileContent) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(fileContent, root);
    if (!parsingSuccessful) {
        throw "Invalid Config file.";
    }
    return root;
}
