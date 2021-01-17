/*
  ==============================================================================

    FilterComponent.cpp
    Created: 22 Dec 2020 1:15:04pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "FilterComponent.h"

#define BEGIN_WITH(x) { \
    auto &_ = x;

#define END_WITH() }

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
    valueTree.addChild(node, type, nullptr);
    valueTree.addListener(this);
    DBG(valueTree.toXmlString());
    
    addAndMakeVisible(filterOn);
    filterOn.setName("filterOn");
    filterOn.setButtonText("Off");
    filterOn.setToggleState(false, dontSendNotification);
    filterOn.addListener(this);
    
    addAndMakeVisible(frequencySlider);
    
    BEGIN_WITH(frequencySlider)
    _.setSliderStyle(Slider::RotaryVerticalDrag);
    _.setRange(20, 20000, 1);
    _.setSkewFactorFromMidPoint(500);
    _.setTextValueSuffix(" Hz");
    _.setMouseDragSensitivity(80);
    END_WITH()

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
    //delete node;
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

void FilterComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                               const Identifier& property)
{
    int fType = treeWhosePropertyHasChanged.getProperty("filterType");
    if(fType == filterType)
    {
        float x = treeWhosePropertyHasChanged.getProperty(property);
        String propertyName = property.toString();
        
        if(propertyName == "frequency")
        {
            filter->setFrequency(x);
        }
        else if(propertyName == "resonance")
        {
            filter->setResonance(x);
        }
        else if(propertyName == "gain")
        {
            filter->setGain(x);
        }
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
        
        if(filter->getFilterMagnitudesReady())
            filterResponseComponent->drawResponseCurve(filter->getFrequencies(), filter->getMagnitudes(), button->getToggleState());
    } 
}
