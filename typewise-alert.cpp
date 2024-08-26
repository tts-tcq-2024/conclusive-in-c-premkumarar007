#include "typewise-alert.h"
#include <stdio.h>

typedef struct {
    int lowerLimit;
    int upperLimit;
} CoolingLimits;

CoolingLimits coolingLimitsTable[] = {
    {0, 35}, // PASSIVE_COOLING
    {0, 45}, // HI_ACTIVE_COOLING
    {0, 40}  // MED_ACTIVE_COOLING
};

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    CoolingLimits limits = coolingLimitsTable[coolingType];
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendTooLowEmail() {
    const char* recepient = "a.b@c.com";
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too low\n");
}

void sendTooHighEmail() {
    const char* recepient = "a.b@c.com";
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too high\n");
}

void sendToEmail(BreachType breachType) {
    void (*emailHandlers[])(void) = {NULL, sendTooLowEmail, sendTooHighEmail};
    if (breachType != NORMAL) {
        emailHandlers[breachType]();
    }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    
    AlertHandler alertHandlers[] = {sendToController, sendToEmail};
    alertHandlers[alertTarget](breachType);
}
