/*
  ==============================================================================

    FilterComponent.cpp
    Created: 22 Dec 2020 1:15:04pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "FilterComponent.h"

FilterComponent::FilterComponent()
{
    frequencySlider.setSliderStyle(Slider::Rotary);
    frequencySlider.setRange(20, 20000);
    frequencySlider.setSkewFactorFromMidPoint(500);

    addAndMakeVisible(frequencySlider);
}

FilterComponent::~FilterComponent()
{
    delete frequencySliderAttachment;
}

void FilterComponent::setValueTree(ValueTree* valueTreeRef)
{
    valueTree = valueTreeRef;
    
    frequencySliderAttachment = new ValueTreeSliderAttachment(*valueTree, &frequencySlider, "frequency");
}

void FilterComponent::paint (juce::Graphics&)
{
    
}

void FilterComponent::resized()
{
    auto bounds = getLocalBounds();
    frequencySlider.setBounds(bounds.removeFromTop(bounds.getHeight() / 3).reduced(UIElementProperties::buttonPadding));
    
}

void FilterComponent::sliderValueChanged (Slider* slider)
{
    
}

void FilterComponent::buttonClicked (Button*)
{
    
}
 
void FilterComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    
}
