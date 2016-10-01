#ifndef PROJECT_LOCATION_H
#define PROJECT_LOCATION_H

#include "../jsoncpp/json/json.h"
#include "../jsoncpp/json/json-forwards.h"

using namespace std;

class Location {

public:
    Location();
    ~Location();

    void setLatitude(double latitude);
    void setLongitude(double longitude);

    double getLatitude();
    double getLongitude();

private:
    double latitude;
    double longitude;
};

#endif //PROJECT_LOCATION_H
