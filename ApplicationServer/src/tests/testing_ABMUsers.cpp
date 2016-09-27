#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include <PersonManager.h>

/// Save a new user to DB
TEST(NewUser, SaveUser) {
    PersonManager*  personManager = new PersonManager();
    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager->savePerson(user);

    delete personManager;
}


/// Save  User that exists already
TEST(UserExists, SaveUserERROR) {
    PersonManager* personManager_ = new PersonManager();
    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager_->savePerson(user);
    EXPECT_THROW(personManager_->savePerson(user), UserAlreadyExistsException);

    delete personManager_;

}

/// Get  User that exists already
TEST(UserExists, GetUserById) {
    PersonManager* personManager_ = new PersonManager();

    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    long id = 1; //Se aumenta en 1 el id después de crearlo.
    personManager_->savePerson(user);

    Person* person = personManager_->getPersonById(id);
    EXPECT_EQ(person->getLastName(), "Rodriguez");

    delete person;
    delete personManager_;

}

///Get User that doesn't exists by Id
TEST(NewUser, GetUserById) {
    PersonManager* personManager_;
    long id;

    personManager_= new PersonManager();
    id = 3;

    EXPECT_THROW(personManager_->getPersonById(id), UserNotFoundException);

    delete personManager_;

}

///Get User that doesn't exists by Mail
TEST(NewUser, GetUserByMail) {
    PersonManager* personManager_;
    std::string user_mail;

    personManager_= new PersonManager();
    user_mail = "cc";

    EXPECT_THROW(personManager_->getPersonByMail(&user_mail), UserNotFoundException);

    delete personManager_;

}

///Get User that exists in DB by Mail
TEST(UserExists, GetUserByMail) {
    PersonManager*  personManager;
    Json::Value user;
    std::string user_mail;

    personManager = new PersonManager();

    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    user_mail = user["email"].asString();

    personManager->savePerson(user);
    Person* person = personManager->getPersonByMail(&user_mail);

    EXPECT_EQ(person->getCity(), "CABA");
    delete personManager;
}

/// Delete User that doesn't exists
TEST(NewUser, DeleteUser) {
    PersonManager* personManager;
    long id;

    id = 3;
    personManager = new PersonManager();

    EXPECT_THROW(personManager->deletePerson(id), UserNotFoundException);

    delete personManager;

}

///Delete User that exists in DB, expects to be deleted by Mail and by Id
TEST(UserExists, DeleteUser) {
    PersonManager* personManager;
    std::string user_mail;
    Json::Value user;
    long id;

    id = 1;
    personManager = new PersonManager();

    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager->savePerson(user);
    personManager->deletePerson(id);

    user_mail = "crodriguez@gmail.com";

    EXPECT_THROW(personManager->getPersonById(id), UserNotFoundException);
    EXPECT_THROW(personManager->getPersonByMail(&user_mail), UserNotFoundException);

    delete personManager;

}