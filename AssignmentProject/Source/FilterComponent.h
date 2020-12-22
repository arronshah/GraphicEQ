/*
  ==============================================================================

    FilterComponent.h
    Created: 22 Dec 2020 1:15:04pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Filter.h"
#include "UIElementProperties.h"
#include <memory>

class FilterComponent : public Component,
                               Slider::Listener,
                               Button::Listener,
                               ComboBox::Listener
{
public:
    FilterComponent();
    ~FilterComponent();
    void paint (juce::Graphics&) override;
    void resized() override;
    void setValueTree(ValueTree* valueTreeRef);
    virtual void sliderValueChanged (Slider* slider) override;
    virtual void buttonClicked (Button*) override;
    virtual void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    
private:
    Slider frequencySlider;
    ComboBox filterType;
    ToggleButton filterOn;
    ValueTree* valueTree {nullptr};
    
    ValueTreeSliderAttachment* frequencySliderAttachment;
    
};
