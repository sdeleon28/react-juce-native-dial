#include <algorithm>
#include <math.h>
#include <assert.h>

#include <juce_graphics/juce_graphics.h>
using namespace juce;

#include "DiscreteDialMarks.h"
#include "DialUtils.h"

#define FONT_HEIGHT 12

DiscreteDialMarks::DiscreteDialMarks(AudioProcessorValueTreeState& _treeState)
    : reactjuce::View()
    , treeState(_treeState)
{
    startTimerHz(30);
    font = Font("lato", FONT_HEIGHT, Font::FontStyleFlags::bold);
}

void DiscreteDialMarks::timerCallback()
{
    repaint();
}

RangedAudioParameter* DiscreteDialMarks::getParam()
{
    String paramName = props["paramId"];
    return treeState.getParameter(paramName);
}

void DiscreteDialMarks::paint (juce::Graphics& g)
{
    auto dialSize = (int) props.getWithDefault("size", 72);
    auto isEnum = (bool) props.getWithDefault("isEnum", false);

    // "componentDidMount"
    if (isEnum && possibleValues.size() == 0)
    {
        int steps = getParam()->getNumSteps();
        float stepSize = 1.0f / (float) (steps - 1);
        float currentValue = 0.f;
        for (int i = 0; i < steps; i++)
        {
            possibleValues.push_back(currentValue);
            currentValue += stepSize;
        }
        labels = *props["labels"].getArray();
    }

    auto bounds = getLocalBounds();
    auto containerHeight = bounds.getHeight();
    auto containerWidth = bounds.getWidth();

    auto left = containerWidth / 2 -  dialSize / 2;
    auto top = containerHeight / 2 -  dialSize / 2;

    auto pivotX = (float) left + (float) dialSize / 2.0f;
    auto pivotY = (float) top + (float) dialSize / 2.0f;

    auto numSteps = getParam()->getNumSteps();

    if (isEnum)
    {
        float normalizedValue;
        String label;
        for (size_t i = 0; i < possibleValues.size(); i++)
        {
            Graphics::ScopedSaveState state(g);
            normalizedValue = possibleValues[i];
            label = labels[(int) i];
            auto ellipseRadius = 2;
            auto angle = DialUtils::calculateAngleForNormalizedValue(normalizedValue, isEnum, numSteps);
            auto markerContainerWidth = containerWidth;
            auto markerContainerLeft = (containerWidth - markerContainerWidth) / 2;
            auto markerContainerHeight = (int) ((float) dialSize * 1.4f);
            auto markerContainerTop = (containerHeight - markerContainerHeight) / 2;
            auto containerRectangle = Rectangle<int>(markerContainerLeft, markerContainerTop, markerContainerWidth, markerContainerHeight);
            auto ellipseX = markerContainerLeft + (markerContainerWidth / 2);
            auto ellipseY = (int) ((float) markerContainerTop * 1.5f);
            AffineTransform markContainerTransform = AffineTransform::rotation(angle, pivotX, pivotY);
            g.addTransform(markContainerTransform);
            g.setColour(Colour::fromString("FF626262"));
            g.fillEllipse((float) ellipseX, (float) ellipseY, (float) ellipseRadius * 2.f, (float) ellipseRadius * 2.f);

            g.setFont(font);
            auto labelTop = markerContainerTop;
            auto labelHeight = FONT_HEIGHT;
            auto labelWidth = (int) ((float) containerWidth * 0.35f);
            int labelLeft;
            auto isOnLeftSide = (float) i < (float) possibleValues.size() / 2.f;
            if (isOnLeftSide)
            {
                labelLeft = (int) ((float) containerWidth * 0.13f);
            }
            else
            {
                labelLeft = (int) ((float) containerWidth * 0.54f);
            }
            AffineTransform labelTransform = AffineTransform::rotation(-angle, (float) ellipseX, (float) ellipseY);
            g.addTransform(labelTransform);
            g.setColour(Colour::fromString("FF626262"));
            Justification justification = isOnLeftSide ? Justification::centredRight : Justification::centredLeft;
            g.drawText(label, labelLeft, labelTop, labelWidth, labelHeight, justification, true);
        }
    }
}
