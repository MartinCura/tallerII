//
// Created by milena on 02/10/16.
//


#include <gtest/gtest.h>
#include "../session/SessionManager.h"
#include "../Exceptions/InvalidTokenException.h"

TEST(SessionToken, trySession) {
    SessionManager* sessionManager = new SessionManager("/tmp/testDB");

    EXPECT_THROW(sessionManager->getSession("alkjdf"), InvalidTokenException);
    sessionManager->destroyDB();
    delete sessionManager;
}
