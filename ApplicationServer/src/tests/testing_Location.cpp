#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "Location.h"

TEST(NewLocation, Empty) {
    Location* location = new Location();
    EXPECT_EQ(location->getLatitude(), 0.0);
    EXPECT_EQ(location->getLongitude(), 0.0);
    delete location;
}

TEST(NewLocation, FillValues) {
    Location* location = new Location();
    location->setLatitude(-58.368368);
    location->setLongitude(-34.617589);
    EXPECT_EQ(location->getLatitude(), -58.368368);
    EXPECT_EQ(location->getLongitude(), -34.617589);
    delete location;
}
