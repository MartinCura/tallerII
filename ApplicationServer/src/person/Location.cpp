#include <cmath>
#include "Location.h"

Location::Location() {
    this->latitude = 0.0;
    this->longitude = 0.0;
}

Location::~Location() {}

bool Location::isEmpty() {
    return this->latitude == 0.0 && this->longitude == 0.0;
}

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

double Location::getDistanceFrom(Location *location) {
    return std::pow(this->latitude - location->getLatitude(), double(2)) + std::pow(this->longitude - location->getLongitude(), double(2));
}
