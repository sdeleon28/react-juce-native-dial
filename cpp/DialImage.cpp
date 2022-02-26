#include <algorithm>
#include <math.h>
#include <assert.h>

#include <juce_graphics/juce_graphics.h>
using namespace juce;

#include "BinaryData.h"
#include "DialImage.h"
#include "DialUtils.h"

DialImage::DialImage(AudioProcessorValueTreeState& _treeState)
    : reactjuce::View()
    , treeState(_treeState)
    , valueAtDragStart(0)
{
    startTimerHz(30);
    dialImage = ImageCache::getFromMemory(BinaryData::dial_png, BinaryData::dial_pngSize);
}

void DialImage::timerCallback()
{
    repaint();
}

float DialImage::linearVerticalGestureMap(
    const MouseEvent& dragEvent
) {
    // TODO: Make this configurable
    float sensitivity = 1.f / 300.f;
    auto dy = (float) (dragEvent.y - dragEvent.getMouseDownY());
    return std::max(0.0f, std::min(1.0f, valueAtDragStart - dy * sensitivity));
}

RangedAudioParameter* DialImage::getParam()
{
    String paramName = props["paramId"];
    return treeState.getParameter(paramName);
}

float DialImage::getNormalizedParamValue()
{
    auto param = getParam();
    return param->getValue();
}

void DialImage::setNormalizedParamValue(float valueToSet)
{
    auto param = getParam();
    param->setValueNotifyingHost(valueToSet);
}

void DialImage::mouseEnter(const juce::MouseEvent& e)
{
    setMouseCursor(MouseCursor::StandardCursorType::UpDownResizeCursor);
    View::mouseEnter(e);
}

void DialImage::mouseExit(const juce::MouseEvent& e)
{
    setMouseCursor(MouseCursor::StandardCursorType::NormalCursor);
    View::mouseExit(e);
}

void DialImage::mouseDown(const juce::MouseEvent& e)
{
    auto param = getParam();
    param->beginChangeGesture();
    valueAtDragStart = getNormalizedParamValue();
    setMouseCursor(MouseCursor::StandardCursorType::NoCursor);
    View::mouseDown(e);
}

void DialImage::mouseDrag(const juce::MouseEvent& e)
{
    setNormalizedParamValue(linearVerticalGestureMap(e));
    View::mouseDrag(e);
}

void DialImage::mouseUp(const juce::MouseEvent& e)
{
    auto param = getParam();
    param->endChangeGesture();
    float x = (float) e.getMouseDownScreenX();
    float y = (float) e.getMouseDownScreenY();
    const_cast <MouseInputSource&> (e.source).setScreenPosition (Point(x, y));
    setMouseCursor(MouseCursor::StandardCursorType::UpDownResizeCursor);
    View::mouseUp(e);
}

void DialImage::paint (juce::Graphics& g)
{
    auto SOURCE_IMAGE_SIZE = 72;

    auto dialSize = (int) props.getWithDefault("size", 72);
    auto bounds = getLocalBounds();
    auto containerHeight = bounds.getHeight();
    auto containerWidth = bounds.getWidth();

    auto left = containerWidth / 2 -  dialSize / 2;
    auto top = containerHeight / 2 -  dialSize / 2;
    bool fillAlphaChannelWithCurrentBrush = false;

    auto pivotX = (float) left + (float) dialSize / 2.0f;
    auto pivotY = (float) top + (float) dialSize / 2.0f;

    auto isEnum = (bool) props.getWithDefault("isEnum", false);
    auto numSteps = getParam()->getNumSteps();
    float angle = DialUtils::calculateAngleForNormalizedValue(getNormalizedParamValue(), isEnum, numSteps);

    {
        Graphics::ScopedSaveState state(g);
        AffineTransform t = AffineTransform::rotation(angle, pivotX, pivotY);
        g.addTransform(t);
        g.drawImage(
            dialImage,
            left,
            top,
            dialSize,
            dialSize,
            0, // Source image left crop
            0, // Source image top crop
            SOURCE_IMAGE_SIZE,
            SOURCE_IMAGE_SIZE,
            fillAlphaChannelWithCurrentBrush
        );
        if (props.getWithDefault("focussed", false))
        {
            // TODO: Make this a prop
            g.setColour(Colours::white);
            g.drawEllipse(
                (float) left,
                (float) top,
                (float) dialSize,
                (float) dialSize,
                2 // TODO: Make circle line thickness a prop
            );
        }
    }
}
