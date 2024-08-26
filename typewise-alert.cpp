#include "typewise-alert.h"
#include <stdio.h>

// Lookup table for cooling type limits
typedef struct {
    int lowerLimit;
    int upperLimit;
} CoolingLimits;

const CoolingLimits coolingLimits[] = {
    {0, 35},  // PASSIVE_COOLING
    {0, 45},  // HI_ACTIVE_COOLING
    {0, 40}   // MED_ACTIVE_COOLING
};

// Function to infer breach type
BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if(value < lowerLimit) {
        return TOO_LOW;
    }
    if(value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

// Function to classify temperature breach based on cooling type
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    CoolingLimits limits = coolingLimits[coolingType];
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

// Function to send alerts based on target
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

    // Lookup table for alert handlers
    void (*alertHandlers[])(BreachType) = {sendToController, sendToEmail};
    alertHandlers[alertTarget](breachType);
}

// Function to send breach information to the controller
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

// Function to send breach information via email
void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\n", recipient);
        printf("Hi, the temperature is too low\n");
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\n", recipient);
        printf("Hi, the temperature is too high\n");
    }
}
