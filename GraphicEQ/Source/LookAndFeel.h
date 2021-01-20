/*
  ==============================================================================

    LookAndFeel.h
    Created: 21 Dec 2020 1:22:47pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class UILookAndFeel : public LookAndFeel_V4
{
public:
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    
    Label* createSliderTextBox (Slider& slider) override;
    void drawLabel (Graphics& g, juce::Label& label) override;
    
};
