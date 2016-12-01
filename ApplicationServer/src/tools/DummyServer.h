#ifndef PROJECT_DUMMYSERVER_H
#define PROJECT_DUMMYSERVER_H

#include <string>

using namespace std;

class DummyServer {
public:
    DummyServer();
    ~DummyServer();
    string getFirebaseDummyResponse();
    string getFacebookDummyResponse();
};

#endif //PROJECT_DUMMYSERVER_H
