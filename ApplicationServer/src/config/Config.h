#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstring>
#include "../jsoncpp/json/json.h"
#include "../Exceptions/MissingConfigurationException.h"

using namespace std;

class Config {

public:
    static const char* NAME_DB;
    static const char* IMG_FOLDER;
    static const char* LOGS_FOLDER;

    static Config* getInstance();
    virtual ~Config();

    void load(string path);
    string get(string key);

private:
    Json::Value configValues;

    static Config* instance;
    Config();
    string loadFile(string path);
    Json::Value parseFile(string fileContent);
};

#endif //PROJECT_CONFIG_H
