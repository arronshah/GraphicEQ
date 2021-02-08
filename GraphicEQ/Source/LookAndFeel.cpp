/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 21 Dec 2020 1:22:47pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "LookAndFeel.h"
#include "UIElementProperties.h"

void UILookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                                    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    
    auto outline = slider.getParentComponent()->findColour(Slider::rotarySliderOutlineColourId);
    
    auto fill    = findColour (Slider::rotarySliderFillColourId);
    auto text    = Colour {0xffd4d4d4};
    auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    int heightWithPadding = height - UIElementProperties::buttonPadding;
    int xWithPadding = x + UIElementProperties::buttonPadding;
    int yWithPadding = y + UIElementProperties::buttonPadding;
    
    if (!slider.isEnabled())
    {
        fill = fill.darker (0.8f);
        outline = outline.darker (0.8f);
        text = text.darker (0.8f);
    }
    else if (slider.hasKeyboardFocus (true))
    {
        g.setColour (Colour {0x0affffff});
        g.fillAll();
        
        fill = fill.brighter (0.01f);
        outline = outline.brighter (0.4f);
    }
    
    g.setColour (fill);
    g.fillEllipse (xWithPadding, yWithPadding, heightWithPadding, heightWithPadding);
    
    auto arcBounds = Rectangle<float> (xWithPadding, yWithPadding, heightWithPadding, heightWithPadding).reduced (height * 0.12f);
    
    Path valueArc;
    valueArc.addArc (arcBounds.getX(),     arcBounds.getY(),
                     arcBounds.getHeight(), arcBounds.getHeight(),
                     rotaryStartAngle, toAngle, true);
    
    g.setColour (outline);
    g.strokePath (valueArc, PathStrokeType (heightWithPadding * 0.14f, PathStrokeType::curved, PathStrokeType::butt));
}

Label* UILookAndFeel::createSliderTextBox (Slider& slider)
{
    auto* l = LookAndFeel_V2::createSliderTextBox (slider);
    
    if (getCurrentColourScheme() == LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == Slider::LinearBar
                                                                              || slider.getSliderStyle() == Slider::LinearBarVertical))
    {
        l->setColour (Label::textColourId, Colours::black.withAlpha (0.7f));
        
    }
    
    l->setColour(Label::outlineColourId, Colours::transparentBlack);
    
    return l;
}

void UILookAndFeel::drawLabel (Graphics& g, juce::Label& label)
{
    if (label.isBeingEdited()) return;
    
    label.setTopLeftPosition(label.getX(), label.getY());
    g.setFont (getLabelFont (label));
    g.setColour (Colour {label.isEnabled()? 0xffd4d4d4 : 0xff757575});
    g.drawText (label.getText(), label.getLocalBounds(), label.getJustificationType(), false);
}
