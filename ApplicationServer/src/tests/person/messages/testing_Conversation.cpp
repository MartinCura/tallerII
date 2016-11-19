#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "../../../person/messages/Conversation.h"

TEST(NewConversation, Empty) {
    Conversation* conversation = new Conversation();
    EXPECT_EQ(conversation->getWith(), 0);
    EXPECT_EQ(conversation->getFirstName(), "");
    EXPECT_EQ(conversation->getLastName(), "");
    EXPECT_EQ(conversation->getUnreadCount(), 0);
    delete conversation;
}

TEST(NewConversation, FillValues) {
    Conversation* conversation = new Conversation();
    conversation->setWith(1);
    conversation->setFirstName("John");
    conversation->setLastName("Doe");
    conversation->setUnreadCount(3);
    EXPECT_EQ(conversation->getWith(), 1);
    EXPECT_EQ(conversation->getFirstName(), "John");
    EXPECT_EQ(conversation->getLastName(), "Doe");
    EXPECT_EQ(conversation->getUnreadCount(), 3);
    delete conversation;
}

TEST(NewConversation, ToJson) {
    Conversation* conversation = new Conversation();
    conversation->setWith(1);
    conversation->setFirstName("John");
    conversation->setLastName("Doe");
    conversation->setUnreadCount(3);
    Json::Value conversationAsJson = conversation->serializeMe();
    EXPECT_EQ(conversationAsJson[Conversation::WITH_KEY].asLargestInt(), 1);
    EXPECT_EQ(conversationAsJson[Conversation::FIRST_NAME_KEY].asString(), "John");
    EXPECT_EQ(conversationAsJson[Conversation::LAST_NAME_KEY].asString(), "Doe");
    EXPECT_EQ(conversationAsJson[Conversation::UNREAD_COUNT_KEY].asInt(), 3);
    delete conversation;
}
