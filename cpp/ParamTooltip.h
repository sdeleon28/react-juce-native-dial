#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <react_juce.h>

class ParamTooltip: public reactjuce::View, public juce::Timer
{
public:
    ParamTooltip(AudioProcessorValueTreeState& treeState);
    void paint (juce::Graphics& g) override;
    Font getFont();
    void timerCallback() override;
    Colour getColor();
    RangedAudioParameter* getParam();
    float getNormalizedParamValue();
    String getParamStringValue();
    TextLayout getTextLayout(float maxWidth);
private:
    AudioProcessorValueTreeState& treeState;
    Font prevFont;
    String prevStringValue;
    Colour prevColor;
};
