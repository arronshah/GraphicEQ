/*
  ==============================================================================

    FilterComponent.cpp
    Created: 22 Dec 2020 1:15:04pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "FilterComponent.h"


FilterComponent::FilterComponent(ValueTree& vt, enum filterType type) :
    valueTree(vt),
    filterType(type)
{
    String filterParams;
    filterParams << "filterParams" << type;
    ValueTree node(filterParams);
    node.setProperty("filterType", type, nullptr);
    node.setProperty("frequency", 440.f, nullptr);
    node.setProperty("resonance", 0.7, nullptr);
    node.setProperty("gain", 1.f, nullptr);
    node.setProperty("state", 0, nullptr);
    valueTree.addChild(node, type, nullptr);
    valueTree.addListener(this);
    DBG(valueTree.toXmlString());
    
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
}

void FilterComponent::sliderValueChanged(Slider* slider)
{
    if (filterResponseComponent != nullptr)
    {
        if(filter->getFilterMagnitudesReady())
        {
            filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), filter->getCurrentState());
            filter->setFilterMagnitudesReady(false);
        }
    }
}

void FilterComponent::updateFilterParameter(String parameterName, float parameterValue)
{
    if(parameterName == "frequency")
    {
        filter->setFrequency(parameterValue);
    }
    else if(parameterName == "resonance")
    {
        filter->setResonance(parameterValue);
    }
    else if(parameterName == "gain")
    {
        filter->setGain(parameterValue);
    }
}

void FilterComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                               const Identifier& property)
{
    int fType = treeWhosePropertyHasChanged.getProperty("filterType");
    if(fType == filterType)
    {
        float propertyValue = treeWhosePropertyHasChanged.getProperty(property);
        updateFilterParameter(property.toString(), propertyValue);
    }
}

void FilterComponent::createValueTreeAttachments(int index)
{
    filterSubTree = valueTree.getChildWithProperty("filterType", index);
    frequencySliderAttachment = new ValueTreeSliderAttachment(filterSubTree, &frequencySlider, "frequency");
    resonanceSliderAttachment = new ValueTreeSliderAttachment(filterSubTree, &resonanceSlider, "resonance");
    gainSliderAttachment = new ValueTreeSliderAttachment(filterSubTree, &gainSlider, "gain");
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
    
    auto row = bounds.getHeight() / 3;
    frequencySlider.setBounds(bounds.removeFromTop(row).reduced(UIElementProperties::buttonPadding));
    resonanceSlider.setBounds(bounds.removeFromTop(row).reduced(UIElementProperties::buttonPadding));
    gainSlider.setBounds(bounds.removeFromTop(row).reduced(UIElementProperties::buttonPadding));
    
    if (filterResponseComponent != nullptr)
        filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), filter->getCurrentState());
}

void FilterComponent::setFilter(Filter* filterRef)
{
    filter = filterRef;
}

void FilterComponent::setFilterResponseComponent(FilterResponseCurveComponent* frcc)
{
    filterResponseComponent = frcc;
}

void FilterComponent::buttonClicked(Button* button)
{
    if(button->getName() == "filterOn")
    {
        if(filterSubTree["state"])
        {
            button->setToggleState(false, dontSendNotification);
            button->setButtonText("Off");
            filterSubTree.setProperty("state", 0, nullptr);
        }
        else
        {
            button->setToggleState(true, dontSendNotification);
            button->setButtonText("On");
            filterSubTree.setProperty("state", 1, nullptr);
        }
        
        filterSubTree.setProperty("state", button->getToggleState(), nullptr);
        filter->setState(filterSubTree["state"]);
        
        if(filter->getFilterMagnitudesReady())
            filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), button->getToggleState());
    } 
}
