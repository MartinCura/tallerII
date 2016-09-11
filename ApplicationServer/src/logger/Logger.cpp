#include "Logger.h"

const char *Logger::ERROR = "error";
const char *Logger::WARN = "warn";
const char *Logger::INFO = "info";
const char *Logger::DEBUG = "debug";

Logger* Logger::instance = NULL;

Logger::Logger() {
    this->logFile.open("ApplicationServer.log", ios::trunc);
    time_t now = time(0);
    this->logFile << "Starting server at ";
    this->logFile << ctime(&now);
    this->logFile << "\n";
    this->logFile.close();
}

Logger::~Logger() {}

Logger* Logger::getInstance() {
    if (!instance) {
        instance = new Logger;
    }
    return instance;
}

void Logger::log(const char *logMode, string message) {
        this->logFile.open("ApplicationServer.log", ios_base::app);
        this->logFile << this->getLogPrefix(logMode);
        this->logFile << message;
        this->logFile << "\n";
        this->logFile.close();
}

string Logger::getLogPrefix(const char *logMode) {
    if (strcmp(logMode, Logger::DEBUG) == 0) {
        return "[DEBUG]: ";
    } else if (strcmp(logMode, Logger::INFO) == 0) {
        return "[INFO]: ";
    } else if (strcmp(logMode, Logger::WARN) == 0) {
        return "[WARN]: ";
    }
    return "[ERROR]: ";
}
