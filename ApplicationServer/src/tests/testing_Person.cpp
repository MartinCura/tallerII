#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "Person.h"

Json::Value generatePersonJson() {
	Json::Value user;
	user["id"] = 1;
	user["first_name"] = "Carlos";
	user["last_name"] = "Rodriguez";
	user["email"] = "crodriguez@gmail.com";
	user["date_of_birth"] = "01/01/1990";
	user["city"] = "CABA";
	user["profile_picture"] = "";
	user["summary"] = "Me gusta el arrte";
	user["location"]["latitude"] = -58.368368;
	user["location"]["longitude"] = -34.617589;

	Json::Value jskill;
	jskill["name"] = "java";
	jskill["description"] = "jdescription";
	jskill["category"] = "developer";

	Json::Value jWorkHistory;
	jWorkHistory["company"] = "IBM";
	jWorkHistory["position_title"] = "Developer";
	jWorkHistory["from_date"] = "2012";
	jWorkHistory["to_date"] = "2014";

	user["skills"].append(jskill);
	user["work_history"].append(jWorkHistory);

	return user;
}

TEST(NewUser, FromJson) {
	Json::Value user = generatePersonJson();
	Person pUser = Person(user);

	EXPECT_EQ(pUser.getFirstName(), "carlos");
	EXPECT_EQ(pUser.getLastName(), "rodriguez");
	EXPECT_EQ(pUser.getEmail(), "crodriguez@gmail.com");
	EXPECT_EQ(pUser.getDateOfBirth(), "01/01/1990" );
	EXPECT_EQ(pUser.getLocation()->getLatitude(), -58.368368);
	EXPECT_EQ(pUser.getLocation()->getLongitude(), -34.617589);

	vector<Skill*> skills = pUser.getSkills();
 	Skill* skill = skills[0];	

 	EXPECT_EQ(skill->getName(), "java");
 	EXPECT_EQ(skill->getDescription(), "jdescription");
 	EXPECT_EQ(skill->getCategory(), "developer");

 	std::vector<WorkHistory*> whs = pUser.getWorkHistory();
 	WorkHistory* wh = whs[0];

	EXPECT_EQ(wh->getCompany(), "IBM");
 	EXPECT_EQ(wh->getPositionTitle(), "Developer");
 	EXPECT_EQ(wh->getFromDate(), "2012");
 	EXPECT_EQ(wh->getToDate(), "2014");
}

TEST(NewUser, EmptyUser) {
	Person user = Person();
	EXPECT_EQ(user.getFirstName(),"");
}

TEST(NewUser, CompleteUser) {
	Person user = Person();
	
	user.setCity("Mar del Plata");
	user.setSummary("Carrera");
	
	EXPECT_EQ(user.getCity(), "mar del plata");
	EXPECT_EQ(user.getSummary(), "Carrera");
}

TEST(User, GetUserJson) {
	Json::Value user = generatePersonJson();
	Person pUser = Person(user);
	Json::Value jUser = pUser.serializeMe();

	EXPECT_EQ(jUser["id"].asInt(), 1);
	EXPECT_EQ(jUser["first_name"].asString(), "carlos");
	EXPECT_EQ(jUser["last_name"].asString(), "rodriguez");
	EXPECT_EQ(jUser["email"].asString(), "crodriguez@gmail.com");
	EXPECT_EQ(jUser["date_of_birth"].asString(), "01/01/1990");
	EXPECT_EQ(jUser["city"].asString(), "caba");
	EXPECT_EQ(jUser["profile_picture"].asString(), "");
	EXPECT_EQ(jUser["summary"].asString(), "Me gusta el arrte");
	EXPECT_EQ(jUser["location"]["latitude"].asDouble(), -58.368368);
	EXPECT_EQ(jUser["location"]["longitude"].asDouble(), -34.617589);

	EXPECT_EQ(jUser["skills"][0]["name"].asString(), "java");
	EXPECT_EQ(jUser["skills"][0]["description"].asString(), "jdescription");
	EXPECT_EQ(jUser["skills"][0]["category"].asString(), "developer");

	EXPECT_EQ(jUser["work_history"][0]["company"].asString(), "IBM");
	EXPECT_EQ(jUser["work_history"][0]["position_title"].asString(), "Developer");
	EXPECT_EQ(jUser["work_history"][0]["from_date"].asString(), "2012");
	EXPECT_EQ(jUser["work_history"][0]["to_date"].asString(), "2014");
}