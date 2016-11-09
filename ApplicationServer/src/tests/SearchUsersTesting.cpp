//
// Created by milena on 08/11/16.
//


#include <gtest/gtest.h>
#include <PersonManager.h>

#define NAME_DB "/tmp/testDB"

/// Save a new user to DB
TEST(SearchEngine, SearchByName) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);
    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager->savePerson(0, user, 0);
    vector<Person*>* user_result = personManager->searchByName("carlos");
    vector<Person*>* user_result2 = personManager->searchByName("rodriguez");

    EXPECT_EQ((*user_result)[0]->getFirstName(),"Carlos");
    EXPECT_EQ((*user_result2)[0]->getFirstName(), "Carlos");

    delete user_result;
    delete user_result2;
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}
