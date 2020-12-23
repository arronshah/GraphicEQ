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
    addAndMakeVisible(frequencySlider);
    frequencySlider.setSliderStyle(Slider::Rotary);
    frequencySlider.setRange(20, 20000);
    frequencySlider.setSkewFactorFromMidPoint(500);

    addAndMakeVisible(resonanceSlider);
    resonanceSlider.setRange(0.1, 18);
    resonanceSlider.setSliderStyle(Slider::Rotary);
    resonanceSlider.setSkewFactorFromMidPoint(1);
    
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(Slider::Rotary);
    gainSlider.setRange(0.f, 1.f);
    
    frequencySlider.addListener(this);
    resonanceSlider.addListener(this);
}

FilterComponent::~FilterComponent()
{
    delete frequencySliderAttachment;
    delete resonanceSliderAttachment;
    delete gainSliderAttachment;
    delete node;
}

void FilterComponent::sliderValueChanged(Slider* slider)
{
    if (filterResponseComponent != nullptr)
        filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes());
}

void FilterComponent::createValueTreeAttachments(int index)
{
    ValueTree* tree = filter->getParameterValueTree();
    node = new ValueTree(tree->getChildWithProperty("filterType", index));
    frequencySliderAttachment = new ValueTreeSliderAttachment(*node, &frequencySlider, "frequency");
    resonanceSliderAttachment = new ValueTreeSliderAttachment(*node, &resonanceSlider, "resonance");
    gainSliderAttachment = new ValueTreeSliderAttachment(*node, &gainSlider, "gain");
}

void FilterComponent::paint (juce::Graphics&)
{
    
}

void FilterComponent::resized()
{
    auto bounds = getLocalBounds();
    auto rowSize = bounds.getHeight() / 3;
    frequencySlider.setBounds(bounds.removeFromTop(rowSize).reduced(UIElementProperties::buttonPadding));
    resonanceSlider.setBounds(bounds.removeFromTop(rowSize).reduced(UIElementProperties::buttonPadding));
    gainSlider.setBounds(bounds.removeFromTop(rowSize).reduced(UIElementProperties::buttonPadding));
    
}

void FilterComponent::setFilter(Filter* filterRef, int index)
{
    filter = filterRef;
    createValueTreeAttachments(index);
}

void FilterComponent::setFilterResponseComponent(FilterResponseCurveComponent* frcc, int index)
{
    filterResponseComponent = frcc;
}
