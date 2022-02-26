#include <juce_graphics/juce_graphics.h>
using namespace juce;

#include "DialUtils.h"

float DialUtils::calculateAngleForNormalizedValue(float value, bool isEnum, int numSteps)
{
    float maxAngleToOneSide;
    if (isEnum)
    {
        switch (numSteps) {
            case 3:
              maxAngleToOneSide = degreesToRadians(45.0f);
              break;
            case 4:
              maxAngleToOneSide = degreesToRadians(70.0f);
              break;
            case 5:
              maxAngleToOneSide = degreesToRadians(90.0f);
              break;
            default:
              maxAngleToOneSide = degreesToRadians(135.0f);
        }
    }
    else
    {
        maxAngleToOneSide = degreesToRadians(135.f);
    }
    float zeroAngle = -maxAngleToOneSide;
    float angleRange = 2 * maxAngleToOneSide;
    return zeroAngle + angleRange * value;
}
