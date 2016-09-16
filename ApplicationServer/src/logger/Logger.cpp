#include "Logger.h"

const char *Logger::FILE_NAME = "ApplicationServer.log";

Logger* Logger::instance = NULL;

Logger::Logger() {
    this->logFile.open(Logger::FILE_NAME, ios_base::app | ios_base::ate);
    time_t now = time(0);
    this->logFile << "\n";
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

void Logger::error(string message) {
    this->log("[ERROR]: ", message);
}

void Logger::warn(string message) {
    this->log("[WARN]: ", message);
}

void Logger::info(string message) {
    this->log("[INFO]: ", message);
}

void Logger::debug(string message) {
    this->log("[DEBUG]: ", message);
}

void Logger::log(string prefix, string message) {
    logMutex.lock();
    this->logFile.open(Logger::FILE_NAME, ios_base::app | ios_base::ate);
    this->logFile << prefix;
    this->logFile << message;
    this->logFile << "\n";
    this->logFile.close();
    logMutex.unlock();
}
