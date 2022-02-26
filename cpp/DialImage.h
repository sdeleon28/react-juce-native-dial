#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <react_juce.h>

class DialImage: public reactjuce::View, public juce::Timer
{
public:
    DialImage(AudioProcessorValueTreeState& treeState);
    void paint (juce::Graphics& g) override;
    void timerCallback() override;
    RangedAudioParameter* getParam();
    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    float getNormalizedParamValue();
    void setNormalizedParamValue(float valueToSet);
protected:
    float linearVerticalGestureMap(
        const MouseEvent& dragEvent
    );
private:
    AudioProcessorValueTreeState& treeState;
    float valueAtDragStart;
    Image dialImage;
};
