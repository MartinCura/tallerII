#ifndef PROJECT_LOGGER_H
#define PROJECT_LOGGER_H

#include <fstream>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <string.h>

using namespace std;

class Logger {

public:
    static const char* ERROR;
    static const char* WARN;
    static const char* INFO;
    static const char* DEBUG;

    static Logger* getInstance();
    void log(const char *logMode, string message);
    virtual ~Logger();

private:
    ofstream logFile;
    static Logger* instance;

    Logger();
    string getLogPrefix(const char* logMode);
};

#endif //PROJECT_LOGGER_H
