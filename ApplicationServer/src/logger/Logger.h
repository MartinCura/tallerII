#ifndef PROJECT_LOGGER_H
#define PROJECT_LOGGER_H

#include <fstream>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include "../config/Config.h"

using namespace std;

class Logger {

public:
    static const char* FILE_EXTENSION;
    static const char* DIRECTORY;

    static Logger* getInstance();
    void error(string message);
    void warn(string message);
    void info(string message);
    void debug(string message);
    virtual ~Logger();

private:
    ofstream logFile;
    static Logger* instance;
    mutex  logMutex;

    Logger();
    void log(string prefix, string message);
    string getTimestamp();
    string getFileName();
};

#endif //PROJECT_LOGGER_H
