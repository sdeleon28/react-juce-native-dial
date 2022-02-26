#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <react_juce.h>

class DiscreteDialMarks: public reactjuce::View, public juce::Timer
{
public:
    DiscreteDialMarks(AudioProcessorValueTreeState& treeState);
    void timerCallback() override;
    RangedAudioParameter* getParam();
    void paint (juce::Graphics& g) override;
private:
    AudioProcessorValueTreeState& treeState;
    std::vector<float> possibleValues;
    Array<var> labels;
    Font font;
};
