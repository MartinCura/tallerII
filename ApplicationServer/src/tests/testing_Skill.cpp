#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "Skill.h"

TEST(NewSkill, Empty) {
    Skill* skill = new Skill();
    EXPECT_EQ(skill->getName(), "");
    EXPECT_EQ(skill->getDescription(), "");
    EXPECT_EQ(skill->getCategory(), "");
    delete skill;
}

TEST(NewSkill, FillValues) {
    Skill* skill = new Skill();
    skill->setName("PHP");
    skill->setDescription("PHP knowledge");
    skill->setCategory("Software");
    EXPECT_EQ(skill->getName(), "PHP");
    EXPECT_EQ(skill->getDescription(), "PHP knowledge");
    EXPECT_EQ(skill->getCategory(), "Software");
    delete skill;
}

TEST(NewSkill, FromJson) {
    Json::Value skillAsJson;
    skillAsJson["name"] = "PHP";
    skillAsJson["description"] = "PHP knowledge";
    skillAsJson["category"] = "Software";
    Skill* skill = new Skill(skillAsJson);
    EXPECT_EQ(skill->getName(), "PHP");
    EXPECT_EQ(skill->getDescription(), "PHP knowledge");
    EXPECT_EQ(skill->getCategory(), "Software");
    delete skill;
}

TEST(NewSkill, ToJson) {
    Skill* skill = new Skill();
    skill->setName("PHP");
    skill->setDescription("PHP knowledge");
    skill->setCategory("Software");
    Json::Value skillAsJson = skill->serializeMe();
    EXPECT_EQ(skillAsJson["name"].asString(), "PHP");
    EXPECT_EQ(skillAsJson["description"].asString(), "PHP knowledge");
    EXPECT_EQ(skillAsJson["category"].asString(), "Software");
    delete skill;
}
