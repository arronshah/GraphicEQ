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
    addAndMakeVisible(filterOn);
    filterOn.setName("filterOn");
    filterOn.setButtonText("Off");
    filterOn.setToggleState(false, dontSendNotification);
    filterOn.addListener(this);
    
    addAndMakeVisible(frequencySlider);
    frequencySlider.setSliderStyle(Slider::RotaryVerticalDrag);
    frequencySlider.setRange(20, 20000, 1);
    frequencySlider.setSkewFactorFromMidPoint(500);
    frequencySlider.setTextValueSuffix(" Hz");
    frequencySlider.setMouseDragSensitivity(80);

    addAndMakeVisible(resonanceSlider);
    resonanceSlider.setRange(0.1, 18, 0.01);
    resonanceSlider.setMouseDragSensitivity(80);
    resonanceSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    resonanceSlider.setSkewFactorFromMidPoint(1);
    
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setRange(0.01, 2.f, 0.01);
    gainSlider.setMouseDragSensitivity(80);
    
    frequencySlider.addListener(this);
    resonanceSlider.addListener(this);
    gainSlider.addListener(this);
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
    {
        filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), filter->getCurrentState());
    }

}

void FilterComponent::createValueTreeAttachments(int index)
{
    ValueTree* tree = filter->getParameterValueTree();
    node = new ValueTree(tree->getChildWithProperty("filterType", index));
    frequencySliderAttachment = new ValueTreeSliderAttachment(*node, &frequencySlider, "frequency");
    resonanceSliderAttachment = new ValueTreeSliderAttachment(*node, &resonanceSlider, "resonance");
    gainSliderAttachment = new ValueTreeSliderAttachment(*node, &gainSlider, "gain");
}

void FilterComponent::paint (juce::Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds().reduced(UIElementProperties::buttonPadding);
    g.setColour(Colours::black);
    g.setOpacity(0.1);
    g.fillRoundedRectangle(bounds.toFloat(), 3.f);
}

void FilterComponent::resized()
{
    auto bounds = getLocalBounds();
    auto filterOnButtonArea = bounds.removeFromTop(bounds.getHeight() / 8);
    auto reducedButtonArea = filterOnButtonArea.reduced(UIElementProperties::buttonPadding);
    filterOn.setBounds(reducedButtonArea.removeFromRight(reducedButtonArea.getWidth() / 4));
    
    auto rowSize = bounds.getHeight() / 3;
    frequencySlider.setBounds(bounds.removeFromTop(rowSize).reduced(UIElementProperties::buttonPadding));
    resonanceSlider.setBounds(bounds.removeFromTop(rowSize).reduced(UIElementProperties::buttonPadding));
    gainSlider.setBounds(bounds.removeFromTop(rowSize).reduced(UIElementProperties::buttonPadding));
    
    if (filterResponseComponent != nullptr)
        filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), filter->getCurrentState());
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

void FilterComponent::buttonClicked(Button* button)
{
    if(button->getName() == "filterOn")
    {
        if(button->getToggleState()){
            button->setToggleState(false, dontSendNotification);
            button->setButtonText("Off");
        }
        else{
            
            button->setToggleState(true, dontSendNotification);
            button->setButtonText("On");
        }
        
        filter->setState(button->getToggleState());
        filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), button->getToggleState());
    }
        
}
