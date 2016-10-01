#include "Location.h"

Location::Location() {
    this->latitude = 0.0;
    this->longitude = 0.0;
}

Location::~Location() {}

void Location::setLatitude(double latitude) {
    this->latitude = latitude;
}

void Location::setLongitude(double longitude) {
    this->longitude = longitude;
}

double Location::getLatitude() {
    return latitude;
}

double Location::getLongitude() {
    return longitude;
}
