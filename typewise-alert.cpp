#include "typewise-alert.h"
#include <stdio.h>

typedef struct {
    int lowerLimit;
    int upperLimit;
} CoolingLimits;

CoolingLimits getCoolingLimits(CoolingType coolingType) {
    switch(coolingType) {
        case PASSIVE_COOLING:
            return (CoolingLimits){0, 35};
        case HI_ACTIVE_COOLING:
            return (CoolingLimits){0, 45};
        case MED_ACTIVE_COOLING:
            return (CoolingLimits){0, 40};
        default:
            return (CoolingLimits){0, 0}; // default case to handle any unexpected input
    }
}

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
    CoolingLimits limits = getCoolingLimits(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    switch(alertTarget) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
    }
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recepient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\n", recepient);
        printf("Hi, the temperature is too low\n");
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\n", recepient);
        printf("Hi, the temperature is too high\n");
    }
}
