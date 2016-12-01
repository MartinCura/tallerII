#include <string>
#include "gtest/gtest.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "WorkHistory.h"

TEST(NewWorkHistory, Empty) {
    WorkHistory* workHistory = new WorkHistory();
    EXPECT_EQ(workHistory->getPositionTitle(), "");
    EXPECT_EQ(workHistory->getCompany(), "");
    EXPECT_EQ(workHistory->getFromDate(), "");
    EXPECT_EQ(workHistory->getToDate(), "");
    delete workHistory;
}

TEST(NewWorkHistory, FillValuesCurrentWork) {
    WorkHistory* workHistory = new WorkHistory();
    workHistory->setPositionTitle("Amazon");
    workHistory->setCompany("Project Manager");
    workHistory->setFromDate("12/2014");
    workHistory->setToDate("");
    EXPECT_EQ(workHistory->getPositionTitle(), "Amazon");
    EXPECT_EQ(workHistory->getCompany(), "Project Manager");
    EXPECT_EQ(workHistory->getFromDate(), "12/2014");
    EXPECT_EQ(workHistory->getToDate(), "");
    delete workHistory;
}

TEST(NewWorkHistory, FillValuesNotCurrentWork) {
    WorkHistory* workHistory = new WorkHistory();
    workHistory->setPositionTitle("Amazon");
    workHistory->setCompany("Project Manager");
    workHistory->setFromDate("12/2012");
    workHistory->setToDate("11/2014");
    EXPECT_EQ(workHistory->getPositionTitle(), "Amazon");
    EXPECT_EQ(workHistory->getCompany(), "Project Manager");
    EXPECT_EQ(workHistory->getFromDate(), "12/2012");
    EXPECT_EQ(workHistory->getToDate(), "11/2014");
    delete workHistory;
}

TEST(NewWorkHistory, FromJsonCurrentWork) {
    Json::Value workHistoryAsJson;
    workHistoryAsJson["position_title"] = "Amazon";
    workHistoryAsJson["company"] = "Project Manager";
    workHistoryAsJson["from_date"] = "12/2012";
    workHistoryAsJson["to_date"] = "";
    WorkHistory* workHistory = new WorkHistory(workHistoryAsJson);
    EXPECT_EQ(workHistory->getPositionTitle(), "Amazon");
    EXPECT_EQ(workHistory->getCompany(), "Project Manager");
    EXPECT_EQ(workHistory->getFromDate(), "12/2012");
    EXPECT_EQ(workHistory->getToDate(), "");
    delete workHistory;
}

TEST(NewWorkHistory, FromJsonNotCurrentWork) {
    Json::Value workHistoryAsJson;
    workHistoryAsJson["position_title"] = "Amazon";
    workHistoryAsJson["company"] = "Project Manager";
    workHistoryAsJson["from_date"] = "12/2012";
    workHistoryAsJson["to_date"] = "11/2014";
    WorkHistory* workHistory = new WorkHistory(workHistoryAsJson);
    EXPECT_EQ(workHistory->getPositionTitle(), "Amazon");
    EXPECT_EQ(workHistory->getCompany(), "Project Manager");
    EXPECT_EQ(workHistory->getFromDate(), "12/2012");
    EXPECT_EQ(workHistory->getToDate(), "11/2014");
    delete workHistory;
}

TEST(NewWorkHistory, ToJsonCurrentWork) {
    WorkHistory* workHistory = new WorkHistory();
    workHistory->setPositionTitle("Amazon");
    workHistory->setCompany("Project Manager");
    workHistory->setFromDate("12/2014");
    workHistory->setToDate("");
    Json::Value workHistoryAsJson = workHistory->serializeMe();
    EXPECT_EQ(workHistoryAsJson["position_title"].asString(), "Amazon");
    EXPECT_EQ(workHistoryAsJson["company"].asString(), "Project Manager");
    EXPECT_EQ(workHistoryAsJson["from_date"].asString(), "12/2014");
    EXPECT_EQ(workHistoryAsJson["to_date"].asString(), "");
    delete workHistory;
}

TEST(NewWorkHistory, ToJsonNotCurrentWork) {
    WorkHistory* workHistory = new WorkHistory();
    workHistory->setPositionTitle("Amazon");
    workHistory->setCompany("Project Manager");
    workHistory->setFromDate("11/2012");
    workHistory->setToDate("11/2014");
    Json::Value workHistoryAsJson = workHistory->serializeMe();
    EXPECT_EQ(workHistoryAsJson["position_title"].asString(), "Amazon");
    EXPECT_EQ(workHistoryAsJson["company"].asString(), "Project Manager");
    EXPECT_EQ(workHistoryAsJson["from_date"].asString(), "11/2012");
    EXPECT_EQ(workHistoryAsJson["to_date"].asString(), "11/2014");
    delete workHistory;
}

