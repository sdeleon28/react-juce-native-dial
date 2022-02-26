#include "ParamTooltip.h"

#define DEFAULT_FONT_FAMILY "lato"
#define DEFAULT_FONT_SIZE 30.f
#define DEFAULT_FONT_STYLE Font::FontStyleFlags::plain

ParamTooltip::ParamTooltip(AudioProcessorValueTreeState& _treeState)
    : reactjuce::View()
    , treeState(_treeState)
{
    startTimerHz(20);
}

Font ParamTooltip::getFont()
{
    String fontFamily = props.getWithDefault("font-family", DEFAULT_FONT_FAMILY);
    float fontSize = props.getWithDefault("font-size", DEFAULT_FONT_SIZE);
    int fontStyle = props.getWithDefault("font-style", DEFAULT_FONT_STYLE);
    return Font(fontFamily, fontSize, fontStyle);
}

void ParamTooltip::timerCallback()
{
    repaint();
}

Colour ParamTooltip::getColor()
{
    String hexColor = props.getWithDefault("color", "#000000");
    String juceHexColor = "FF" + hexColor.substring(1, hexColor.length());
    return Colour::fromString(juceHexColor);
}

RangedAudioParameter* ParamTooltip::getParam()
{
    // TODO: Typecheck React component to make this mandatory
    String paramName = props["param"];
    return treeState.getParameter(paramName);
}

float ParamTooltip::getNormalizedParamValue()
{
    auto param = getParam();
    return param->getValue();
}

String ParamTooltip::getParamStringValue()
{
    RangedAudioParameter* param = getParam();
    return param->getText(getNormalizedParamValue(), 0);
}

TextLayout ParamTooltip::getTextLayout(float maxWidth)
{
    Colour color = getColor();
    String text = getParamStringValue();
    AttributedString as (text);
    TextLayout tl;
    as.setLineSpacing(1.0f);
    as.setFont(getFont());
    as.setColour(color);
    as.setJustification(Justification::centred);
    tl.createLayout(as, maxWidth);
    return tl;
}

void ParamTooltip::paint (juce::Graphics& g)
{
    auto floatBounds = getLocalBounds().toFloat();
    View::paint(g);
    getTextLayout(floatBounds.getWidth()).draw(g, floatBounds);
    prevFont = getFont();
    prevStringValue = getParamStringValue();
    prevColor = getColor();
}
