#ifndef PROJECT_MISSINGCONFIGURATIONEXCEPTION_H
#define PROJECT_MISSINGCONFIGURATIONEXCEPTION_H

#include <string>

using namespace std;

class MissingConfigurationException: public exception {

public:
    MissingConfigurationException(string configKey) {
        this->configKey = configKey;
    }
    ~MissingConfigurationException() {}

    string getMessage() const {
        return "Missing " + this->configKey + " configuration";
    }

private:
    string configKey;
};

#endif //PROJECT_MISSINGCONFIGURATIONEXCEPTION_H
