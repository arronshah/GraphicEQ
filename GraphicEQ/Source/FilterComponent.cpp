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
    node.setProperty("gain", 0.f, nullptr);
    node.setProperty("state", 0, nullptr);
    valueTree.addChild(node, type, nullptr);
    valueTree.addListener(this);
    DBG(valueTree.toXmlString());
    filterSubTree = valueTree.getChildWithProperty("filterType", type);
    
    addAndMakeVisible(filterOn);
    filterOn.setName("filterOn");
    filterOn.setButtonText("Off");
    filterOn.setToggleState(false, dontSendNotification);
    filterOn.addListener(this);
    filterOn.getToggleStateValue().referTo(filterSubTree.getPropertyAsValue("state", nullptr));
    
    frequencySlider.setName("frequencySlider");
    frequencySlider.setSliderStyle(Slider::RotaryVerticalDrag);
    frequencySlider.setRange(20, 20000, 1);
    frequencySlider.setSkewFactorFromMidPoint(500);
    frequencySlider.setTextValueSuffix(" Hz");
    frequencySlider.setMouseDragSensitivity(80);
    addAndMakeVisible(frequencySlider);

    resonanceSlider.setName("resonanceSlider");
    resonanceSlider.setRange(0.1, 18, 0.01);
    resonanceSlider.setMouseDragSensitivity(80);
    resonanceSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    resonanceSlider.setSkewFactorFromMidPoint(1);
    addAndMakeVisible(resonanceSlider);
    
    gainSlider.setName("gainSlider");
    gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setRange(-24.f, 24.f, 0.01);
    gainSlider.setMouseDragSensitivity(80);
    addAndMakeVisible(gainSlider);
    
    frequencySlider.addListener(this);
    resonanceSlider.addListener(this);
    gainSlider.addListener(this);
    
    frequencySliderAttachment = new ValueTreeSliderAttachment(filterSubTree, &frequencySlider, "frequency");
    resonanceSliderAttachment = new ValueTreeSliderAttachment(filterSubTree, &resonanceSlider, "resonance");
    gainSliderAttachment = new ValueTreeSliderAttachment(filterSubTree, &gainSlider, "gain");
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
        if(filter->getCurrentState())
        {
            if(filter->getFilterMagnitudesReady())
            {
                averageFilterResponseCurveComponent->setMagnitudes(filter->getMagnitudes(), filterType);
                drawResponseCurves();
                
                filterResponseComponent->drawResponseCurveHandle(filterSubTree["frequency"], filterSubTree["gain"]);
                
                filter->setFilterMagnitudesReady(false);
            }    
        }
    }
}

void FilterComponent::updateFilterParameter(String parameterName, float parameterValue)
{
    if(parameterName == "frequency")
        filter->setFrequency(parameterValue);
    else if(parameterName == "resonance")
        filter->setResonance(parameterValue);
    else if(parameterName == "gain")
        filter->setGain(Decibels::decibelsToGain(parameterValue));
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

void FilterComponent::paint (juce::Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds().reduced(UIElementProperties::buttonPadding);
    g.setColour(Colour::fromRGB(18, 18, 18).withLightness(10));
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
    {
        drawResponseCurves();
    }
}

void FilterComponent::setFilter(Filter* filterRef)
{
    filter = filterRef;
}

void FilterComponent::setFilterResponseComponent(FilterResponseCurveComponent* frcc)
{
    filterResponseComponent = frcc;
    filterResponseComponent->setValueTree(&filterSubTree);
}

void FilterComponent::buttonClicked(Button* button)
{
    DBG(valueTree.toXmlString());
    if(button->getName() == "filterOn")
    {
        if(button->getToggleState())
        {
            button->setToggleState(false, dontSendNotification);
            button->setButtonText("Off");
        }
        else
        {
            button->setToggleState(true, dontSendNotification);
            button->setButtonText("On");
        }
        
        filter->setState(button->getToggleState());
        
        if(filter->getFilterMagnitudesReady())
        {
            if(!filter->getCurrentState())
                averageFilterResponseCurveComponent->resetMagnitudes(filterType);
            else
                averageFilterResponseCurveComponent->setMagnitudes(filter->getMagnitudes(), filterType);
            
            drawResponseCurves();
        }
    } 
}

void FilterComponent::drawResponseCurves()
{
    averageFilterResponseCurveComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes());
    filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes());
}

void FilterComponent::setAverageFilterResponseComponent(AverageFilterResponseCurveComponent* afrcc)
{
    averageFilterResponseCurveComponent = afrcc;
}
