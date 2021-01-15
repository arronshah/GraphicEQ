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
#include "Filter.h"

class FilterComponent : public Component,
                        public Slider::Listener,
                        public Button::Listener,
                        public ValueTree::Listener
{
public:
    FilterComponent(ValueTree& vt, enum filterType type);
    ~FilterComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;
    void createValueTreeAttachments(int index);
    void setFilter(Filter* filterRef, int index);
    void sliderValueChanged(Slider* slider) override;
    void setFilterResponseComponent(FilterResponseCurveComponent* frcc, int index);
    void buttonClicked(Button* button) override;
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    
private:
    Slider frequencySlider;
    Slider gainSlider;
    Slider resonanceSlider;
    TextButton filterOn;
    
    ValueTreeSliderAttachment* frequencySliderAttachment;
    ValueTreeSliderAttachment* resonanceSliderAttachment;
    ValueTreeSliderAttachment* gainSliderAttachment;
    
    FilterResponseCurveComponent* filterResponseComponent {nullptr};
    Filter* filter {nullptr};
    
    ValueTree filterSubTree;
    ValueTree& valueTree;
    
    float filterType = 0;;
    
};
