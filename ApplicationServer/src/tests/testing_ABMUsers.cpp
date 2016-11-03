#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include "../Exceptions/InvalidPasswordException.h"
#include "../session/SessionManager.h"
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

    personManager->savePerson(0, user, 0);

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

    personManager_->savePerson(0, user, 0);
    EXPECT_THROW(personManager_->savePerson(0, user, 0), UserAlreadyExistsException);


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

    long id = personManager_->savePerson(0, user, 4);

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

    personManager->savePerson(0, user, 3);
    Person* person = personManager->getPersonByMail(&user_mail);

    EXPECT_EQ(person->getCity(), "CABA");

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
    user["password"] = "123";
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    id = personManager->savePerson(0, user, 1);
    personManager->deletePerson(id);

    user_mail = "crodriguez@gmail.com";

    EXPECT_THROW(personManager->getPersonById(id), UserNotFoundException);
    EXPECT_THROW(personManager->getPersonByMail(&user_mail), UserNotFoundException);


    personManager->destroyDB();
    delete personManager;

}

TEST(PersonManagerTest, GetAllUsers) {
    Json::Value user;
    long userId;
    PersonManager *personManager;
    vector<long>* getAllUsersResult;
    unsigned long getAllUsersResultSize;
    bool existsId;

    personManager = new PersonManager(NAME_DB);

    for( int i = 0 ; i < 5 ; i++) {

        user["id"] = 0;
        user["password"] = "123";
        user["first_name"] = "Carlos";
        user["last_name"] = "Rodriguez";
        user["email"] = std::to_string(i) + "@gmail.com";
        user["date_of_birth"] = "01/01/1990";
        user["city"] = "CABA";
        user["profile_picture"] = "";
        user["summary"] = "Me gusta el arrte";

        userId = personManager->savePerson(0, user, i);

        sleep(1);

        getAllUsersResult = personManager->getAllUsersIds();

        getAllUsersResultSize = getAllUsersResult->size();

        EXPECT_EQ(getAllUsersResultSize,i+1);

        existsId = std::find(getAllUsersResult->begin(), getAllUsersResult->end(), userId) != getAllUsersResult->end();
        EXPECT_TRUE(existsId);

        delete getAllUsersResult;

    }

    personManager->destroyDB();
    delete personManager;
}

TEST(PersonManagerTest, loginOK){
    SessionManager* sessionManager;
    PersonManager* personManager;
    Json::Value user;


    personManager = new PersonManager(NAME_DB);

    user["id"] = 0;
    user["password"] = "123";
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager->savePerson(0, user, 1);
    delete personManager;

    sessionManager = new SessionManager(NAME_DB);

    sessionManager->login("crodriguez@gmail.com", "123");

    sessionManager->destroyDB();

    delete sessionManager;

}


TEST(PersonManagerTest, loginUserNotExists){
    SessionManager* sessionManager;

    sessionManager = new SessionManager(NAME_DB);

    EXPECT_THROW(sessionManager->login("crodriguez@gmail.com", "123"), UserNotFoundException);

    sessionManager->destroyDB();
    delete sessionManager;

}

TEST(PersonManagerTest, loginWrongPassword){
    PersonManager* personManager;
    SessionManager* sessionManager;
    Json::Value user;

    personManager = new PersonManager(NAME_DB);

    user["id"] = 0;
    user["password"] = "123";
    user["first_name"] = "Carlos";
    user["last_name"] = "Rodriguez";
    user["email"] = "crodriguez@gmail.com";
    user["date_of_birth"] = "01/01/1990";
    user["city"] = "CABA";
    user["profile_picture"] = "";
    user["summary"] = "Me gusta el arrte";

    personManager->savePerson(0, user, 0);

    delete personManager;


    sessionManager = new SessionManager(NAME_DB);
    EXPECT_THROW(sessionManager->login("crodriguez@gmail.com", "dios"), InvalidPasswordException);

    sessionManager->destroyDB();
    delete sessionManager;

}