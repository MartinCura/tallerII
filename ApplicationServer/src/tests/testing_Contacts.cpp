#include <string>
#include "gtest/gtest.h"
#include "../jsoncpp/json/json.h"
#include "../person/PersonManager.h"
#include "../Exceptions/UserAlreadyExistsException.h"

#define NAME_DB "/tmp/testDB"

void savePersonOne(DBWrapper* db) {

    PersonManager*  personManager = new PersonManager(db);
    try {
        Person* person = new Person();
        person->setId(0);
        person->setFirstName("John");
        person->setLastName("Doe");
        person->setEmail("john@doe.com");
        person->setPassword("123");
        person->setDateOfBirth("01/01/1990");
        person->setCity("Buenos Aires");
        person->setSummary("Hi, this is John Doe");
        person->setLocation(-58.368368, -34.617589);
        personManager->savePerson(person->serializeMe(), (long) 1);
        delete person;
    } catch (UserAlreadyExistsException &exception) {}
    delete personManager;
}

void savePersonTwo(DBWrapper* db) {


    PersonManager*  personManager = new PersonManager(db);
    try {
        Person* person = new Person();
        person->setId(0);
        person->setFirstName("Jane");
        person->setLastName("Doe");
        person->setEmail("jane@doe.com");
        person->setPassword("123");
        person->setDateOfBirth("01/11/1991");
        person->setCity("Madrid");
        person->setSummary("Hi, this is Jane Doe");
        person->setLocation(-58.368368, -34.617589);
        personManager->savePerson(person->serializeMe(), (long) 2);
        delete person;
    } catch (UserAlreadyExistsException &exception) {}
    delete personManager;
}

void requestContact(DBWrapper* db) {
    Json::Value relation;
    relation["action"] = "add_contact";
    relation["author_id"] = 1;
    relation["contact_id"] = 2;

    PersonManager*  personManager = new PersonManager(db);
    try {
        personManager->saveOrUpdateRelation(relation);
    } catch (exception &exception) {}
    delete personManager;
}

void acceptContact(DBWrapper* db) {
    Json::Value relation;
    relation["action"] = "accept_contact";
    relation["author_id"] = 1;
    relation["contact_id"] = 2;

    PersonManager*  personManager = new PersonManager(db);
    try {
        personManager->saveOrUpdateRelation(relation);
    } catch (exception &exception) {}
    delete personManager;
}

void rejectContact(DBWrapper* db) {
    Json::Value relation;
    relation["action"] = "remove_contact";
    relation["author_id"] = 1;
    relation["contact_id"] = 2;
    PersonManager *personManager = new PersonManager(db);
    try {
        personManager->saveOrUpdateRelation(relation);
    } catch (exception &exception) {}
    delete personManager;
}

TEST(Contacts, Empty) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    savePersonOne(db);
    PersonManager*  personManager = new PersonManager(db);
    try {
        vector<Contact*> contacts = personManager->getContactsByUserId(1);
        ASSERT_EQ(contacts.size(), 0);
    } catch (UserNotFoundException &exception) {
        ASSERT_TRUE(false) << "User 1 not found";
    }



    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
}

TEST(Contacts, NotFound) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);savePersonOne(db);
    PersonManager*  personManager = new PersonManager(db);
    try {
        vector<Contact*> contacts = personManager->getContactsByUserId(100);
        ASSERT_EQ(contacts.size(), 0);
    } catch (UserNotFoundException &exception) {
        ASSERT_TRUE(true);
    }

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}

TEST(Contacts, RequestContact) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    savePersonOne(db);
    savePersonTwo(db);
    requestContact(db);
    PersonManager*  personManager = new PersonManager(db);
    try {
        vector<Contact*> contacts = personManager->getContactsByUserId(1);
        ASSERT_EQ(contacts.size(), 1);
        Contact* contact = contacts[0];
        Person* person = personManager->getUserById(contact->getUserId());
        ASSERT_EQ(person->getId(), 2);
        ASSERT_EQ(contact->getStatus(), "requested");
        delete contact;
        delete person;
    } catch (UserNotFoundException &exception) {
        ASSERT_TRUE(false) << "User not found";
    }

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}

TEST(Contacts, AcceptContact) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    savePersonOne(db);
    savePersonTwo(db);
    acceptContact(db);
    PersonManager*  personManager = new PersonManager(db);
    try {
        vector<Contact*> contacts = personManager->getContactsByUserId(1);
        ASSERT_EQ(contacts.size(), 1);
        Contact* contact = contacts[0];
        Person* person = personManager->getUserById(contact->getUserId());
        ASSERT_EQ(person->getId(), 2);
        ASSERT_EQ(contact->getStatus(), "active");
        delete contact;
        delete person;
    } catch (UserNotFoundException &exception) {
        ASSERT_TRUE(false) << "User not found";
    }

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}

TEST(Contacts, RejectContact) {
    string* namedb = new string();
    *namedb = NAME_DB;
    DBWrapper* db = DBWrapper::openDb(namedb);
    savePersonOne(db);
    savePersonTwo(db);
    rejectContact(db);
    PersonManager*  personManager = new PersonManager(db);
    try {
        vector<Contact*> contacts = personManager->getContactsByUserId(1);
        ASSERT_EQ(contacts.size(), 0);
    } catch (UserNotFoundException &exception) {
        ASSERT_TRUE(false) << "User not found";
    }

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete personManager;
    delete namedb;
}
