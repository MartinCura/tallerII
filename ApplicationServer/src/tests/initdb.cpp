//
// Created by milena on 01/10/16.
//

#include <gtest/gtest.h>
#include <PersonManager.h>
#include "../Exceptions/UserAlreadyExistsException.h"


#define NAME_DB "/tmp/appDB/"

TEST(InitDB, init) {
    PersonManager* personManager = new PersonManager(NAME_DB);

    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    Json::Value work;
    work["position_title"] = "QA";
    work["company"] = "Microsofg";
    work["from_date"] = "30/10/1993";
    work["to_date"] = "30/09/2015";

    user["work_history"].append(work);


    Json::Value skill;
    skill["name"] = "Php";
    skill["description"] = "Php programming language";
    skill["category"] = "software";

    user["skills"].append(skill);

    try {
        long usr_id = personManager->savePerson(user, (long)1);
    } catch (UserAlreadyExistsException& exception){}

    delete personManager;

}