#include <gtest/gtest.h>
#include "typewise-alert.h"

// Test the inferBreach function
TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(inferBreach(25, 20, 30), NORMAL);    // Value within limits
    EXPECT_EQ(inferBreach(19, 20, 30), TOO_LOW);   // Value below lower limit
    EXPECT_EQ(inferBreach(31, 20, 30), TOO_HIGH);  // Value above upper limit
}

// Test the classifyTemperatureBreach function for PASSIVE_COOLING
TEST(TypeWiseAlertTestSuite, ClassifiesBreachForPassiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 0), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 35), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);
}

// Test the classifyTemperatureBreach function for HI_ACTIVE_COOLING
TEST(TypeWiseAlertTestSuite, ClassifiesBreachForHiActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 0), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 45), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
}

// Test the classifyTemperatureBreach function for MED_ACTIVE_COOLING
TEST(TypeWiseAlertTestSuite, ClassifiesBreachForMedActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 0), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 40), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
}

// Mocking print statements for testing the alerting functions
class TypeWiseAlertTestSuiteWithOutput : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect stdout to a stringstream for testing output
        testing::internal::CaptureStdout();
    }

    void TearDown() override {
        // Stop capturing stdout and clear the buffer
        std::string output = testing::internal::GetCapturedStdout();
    }
};


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
