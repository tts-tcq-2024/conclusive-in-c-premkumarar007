#include "typewise-alert.h"
#include <stdio.h>

typedef struct {
    int lowerLimit;
    int upperLimit;
} CoolingLimits;

const CoolingLimits coolingLimits[] = {
    {0, 35},  // PASSIVE_COOLING
    {0, 45},  // HI_ACTIVE_COOLING
    {0, 40}   // MED_ACTIVE_COOLING
};


BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if(value < lowerLimit) {
        return TOO_LOW;
    }
    if(value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    CoolingLimits limits = coolingLimits[coolingType];
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}


void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    if (alertTarget != TO_CONTROLLER && alertTarget != TO_EMAIL) {
        return; // Invalid alertTarget, do nothing
    }

    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

    void (*alertHandlers[])(BreachType) = {sendToController, sendToEmail};
    alertHandlers[alertTarget](breachType);
}


void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    if (breachType >= NORMAL && breachType <= TOO_HIGH) {
        printf("%x : %x\n", header, breachType);
    }
}

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

