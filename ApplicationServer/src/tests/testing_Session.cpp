//
// Created by milena on 02/10/16.
//


#include <gtest/gtest.h>
#include "../session/SessionManager.h"

TEST(SessionToken, openNewSession) {
    SessionManager* sessionManager = new SessionManager("/tmp/testDB");
    std::string session_token = sessionManager->getNewToken("milenafarotto@gmail.com");

    EXPECT_EQ(sessionManager->checkSession(session_token), "milenafarotto@gmail.com");
    sessionManager->destroyDB();
    delete sessionManager;
}

TEST(SessionToken, trySession) {
    SessionManager* sessionManager = new SessionManager("/tmp/testDB");

    EXPECT_THROW(sessionManager->checkSession("alkjdf"), std::exception);
    sessionManager->destroyDB();
    delete sessionManager;
}
