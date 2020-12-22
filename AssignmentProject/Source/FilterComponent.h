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
#include "FilterResponseCurveComponent.h"

class FilterComponent : public Component,
                        public Slider::Listener
{
public:
    FilterComponent();
    ~FilterComponent();
    void paint (juce::Graphics&) override;
    void resized() override;
    void createValueTreeAttachments(ValueTree* valueTreeRef);
    void sliderValueChanged(Slider* slider) override;
    void setFilterResponseComponent(FilterResponseCurveComponent* frcc);
    
private:
    Slider frequencySlider;
    Slider gainSlider;
    Slider resonanceSlider;
    ComboBox filterType;
    ToggleButton filterOn;
    
    ValueTreeSliderAttachment* frequencySliderAttachment;
    ValueTreeSliderAttachment* resonanceSliderAttachment;
    ValueTreeSliderAttachment* gainSliderAttachment;
    
    FilterResponseCurveComponent* filterResponseComponent {nullptr};
    
    int x = 1;
    
};
