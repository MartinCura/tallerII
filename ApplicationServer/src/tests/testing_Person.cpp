#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "Person.h"
#include <iostream>


TEST(NewUser, FromJson) {

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
	EXPECT_EQ(pUser.getLastName(), "Rodriguez");
	EXPECT_EQ(pUser.getEmail(), "crodriguez@gmail.com");
	EXPECT_EQ(pUser.getDateOfBirth(), "01/01/1990" );

;
}

TEST(NewUser, EmptyUser) {
	Person user = Person();
	EXPECT_EQ(user.getFirstName(),"");
}

TEST(NewUser, CompleteUser) {

	Person user = Person();
	
	user.setCity("Mar del Plata");
	user.setSummary("Carrera");
	
	EXPECT_EQ(user.getCity(), "Mar del Plata");
	EXPECT_EQ(user.getSummary(), "Carrera");
}

TEST(User, GetUserJson) {

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

	Json::Value jUser = pUser.serializeMe();

	EXPECT_EQ(jUser["id"].asInt(), 1);
	EXPECT_EQ(jUser["first_name"].asString(), "Carlos");
	EXPECT_EQ(jUser["last_name"].asString(), "Rodriguez");
	EXPECT_EQ(jUser["email"].asString(), "crodriguez@gmail.com");
	EXPECT_EQ(jUser["date_of_birth"].asString(), "01/01/1990");
	EXPECT_EQ(jUser["city"].asString(), "CABA");
	EXPECT_EQ(jUser["profile_picture"].asString(), "");
	EXPECT_EQ(jUser["summary"], "Me gusta el arrte");
}