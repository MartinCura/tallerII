#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include <PersonManager.h>

#define NAME_DB "/tmp/testDB/"

/// Save a new user to DB
TEST(NewUser, SaveUser) {
    PersonManager*  personManager = new PersonManager(NAME_DB);
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

    personManager->destroyDB();
    delete personManager;
}


/// Save  User that exists already
TEST(UserExists, SaveUserERROR) {
    PersonManager* personManager_ = new PersonManager(NAME_DB);
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


    personManager_->destroyDB();
    delete personManager_;

}

/// Get  User that exists already
TEST(UserExists, GetUserById) {
    PersonManager* personManager_ = new PersonManager(NAME_DB);

    Json::Value user;
    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    long id = personManager_->savePerson(user);

    Person* person = personManager_->getPersonById(id);
    EXPECT_EQ(person->getLastName(), "Rodriguez");


    personManager_->destroyDB();
    delete person;
    delete personManager_;

}

///Get User that doesn't exists by Id
TEST(NewUser, GetUserById) {
    PersonManager* personManager_;
    long id;

    personManager_= new PersonManager(NAME_DB);
    id = 9847899876;

    EXPECT_THROW(personManager_->getPersonById(id), UserNotFoundException);


    personManager_->destroyDB();
    delete personManager_;

}

///Get User that doesn't exists by Mail
TEST(NewUser, GetUserByMail) {
    PersonManager* personManager_;
    std::string user_mail;

    personManager_= new PersonManager(NAME_DB);
    user_mail = "cc";

    EXPECT_THROW(personManager_->getPersonByMail(&user_mail), UserNotFoundException);


    personManager_->destroyDB();
    delete personManager_;

}

///Get User that exists in DB by Mail
TEST(UserExists, GetUserByMail) {
    PersonManager*  personManager;
    Json::Value user;
    std::string user_mail;

    personManager = new PersonManager(NAME_DB);

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

    personManager->getAllUsersIds();

        personManager->destroyDB();
        delete personManager;
}

/// Delete User that doesn't exists
TEST(NewUser, DeleteUser) {
    PersonManager* personManager;
    long id;

    id = 3876676667899;
    personManager = new PersonManager(NAME_DB);

    EXPECT_THROW(personManager->deletePerson(id), UserNotFoundException);


    personManager->destroyDB();
    delete personManager;

}

///Delete User that exists in DB, expects to be deleted by Mail and by Id
TEST(UserExists, DeleteUser) {
    PersonManager* personManager;
    std::string user_mail;
    Json::Value user;
    long id;

    personManager = new PersonManager(NAME_DB);

    user["id"] = 0;
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    id = personManager->savePerson(user);
    personManager->deletePerson(id);

    user_mail = "crodriguez@gmail.com";

    EXPECT_THROW(personManager->getPersonById(id), UserNotFoundException);
    EXPECT_THROW(personManager->getPersonByMail(&user_mail), UserNotFoundException);


    personManager->destroyDB();
    delete personManager;

}