//
// Created by milena on 02/10/16.
//


#include <gtest/gtest.h>
#include "../session/SessionManager.h"
#include "../Exceptions/InvalidTokenException.h"

TEST(SessionToken, trySession) {
    std::string* namedb = new std::string();
    *namedb = "/tmp/testDB";

    DBWrapper* db = DBWrapper::openDb(namedb);
    SessionManager* sessionManager = new SessionManager(db);

    EXPECT_THROW(sessionManager->getSession("alkjdf"), InvalidTokenException);

    db->destroyDB(namedb);
    DBWrapper::ResetInstance();
    delete sessionManager;
    delete namedb;
}
