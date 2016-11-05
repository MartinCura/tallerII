#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include "../Exceptions/InvalidPasswordException.h"
#include "../session/SessionManager.h"
#include <PersonManager.h>

#define NAME_DB "/tmp/testDB"

/// Save a new user to DB
TEST(NewUser, SaveUser) {
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

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}


/// Save  User that exists already
TEST(UserExists, SaveUserERROR) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);PersonManager*  personManager_ = new PersonManager(db);
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


    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager_;
    delete namedb;

}

/// Get  User that exists already
TEST(UserExists, GetUserById) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
      
    PersonManager*  personManager_ = new PersonManager(db);
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
     db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete person;
    delete personManager_;
    delete namedb;

}

///Get User that doesn't exists by Id
TEST(NewUser, GetUserById) {
    long id;
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
      
    PersonManager*  personManager_ = new PersonManager(db);
    id = 9847899876;

    EXPECT_THROW(personManager_->getPersonById(id), UserNotFoundException);


    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager_;
    delete namedb;

}

///Get User that doesn't exists by Mail
TEST(NewUser, GetUserByMail) {
    std::string user_mail;

    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
      
    PersonManager*  personManager_ = new PersonManager(db);
    user_mail = "cc";

    EXPECT_THROW(personManager_->getPersonByMail(&user_mail), UserNotFoundException);

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager_;
    delete namedb;

}

///Get User that exists in DB by Mail
TEST(UserExists, GetUserByMail) {
    Json::Value user;
    std::string user_mail;

    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);
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

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}

/// Delete User that doesn't exists
TEST(NewUser, DeleteUser) {
    long id;

    id = 3876676667899;
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);
    EXPECT_THROW(personManager->deletePerson(id), UserNotFoundException);

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;

}

///Delete User that exists in DB, expects to be deleted by Mail and by Id
TEST(UserExists, DeleteUser) {
    std::string user_mail;
    Json::Value user;
    long id;

    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);

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

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;

}

TEST(PersonManagerTest, GetAllUsers) {
    Json::Value user;
    long userId;
    vector<long>* getAllUsersResult;
    unsigned long getAllUsersResultSize;
    bool existsId;

    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);

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

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
}

TEST(PersonManagerTest, loginOK){
    SessionManager* sessionManager;
    Json::Value user;

    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager*  personManager = new PersonManager(db);
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

    sessionManager = new SessionManager(db);

    sessionManager->login("crodriguez@gmail.com", "123");

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();

    delete sessionManager;
    delete namedb;

}


TEST(PersonManagerTest, loginUserNotExists){
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    SessionManager* sessionManager = new SessionManager(db);


    EXPECT_THROW(sessionManager->login("crodriguez@gmail.com", "123"), UserNotFoundException);


    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete sessionManager;

}

TEST(PersonManagerTest, loginWrongPassword){

    SessionManager* sessionManager;
    Json::Value user;
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    PersonManager* personManager = new PersonManager(db);


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


    sessionManager = new SessionManager(db);
    EXPECT_THROW(sessionManager->login("crodriguez@gmail.com", "dios"), InvalidPasswordException);
    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete sessionManager;
    delete namedb;

}