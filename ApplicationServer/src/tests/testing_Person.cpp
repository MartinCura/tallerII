#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "Person.h"
#include <iostream>


TEST(User, NewUserFromJson) {

	Json::Value user;
	user["id"] = 1;
	user["first_name"] = "Carlos";
	user["last_name"] = "Rodriguez";
	user["email"] = "crodriguez@gmail.com";
	user["date_of_birth"] = "01/01/1990";
	user["city"] = "CABA";
	user["profile_picture"] = "";
	user["summary"] = "Me gusta el arrte";

	Person pUser = Person(user);
	
	EXPECT_EQ(pUser.getFirstName(), "Carlos");
}

TEST(User, NewUser) {
	Person user = Person();
	EXPECT_EQ(user.getFirstName(),"");
}

