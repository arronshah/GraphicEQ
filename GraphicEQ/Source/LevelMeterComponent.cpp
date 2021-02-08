/*
  ==============================================================================

    LevelMeterComponent.cpp
    Created: 26 Dec 2020 3:00:15pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "LevelMeterComponent.h"

LevelMeterComponent::LevelMeterComponent()
{
    peakLevel = 0.f;
}

LevelMeterComponent::~LevelMeterComponent()
{
    
}

void LevelMeterComponent::paint(Graphics& g)
{
    auto height = getHeight() - 20;
    auto bounds = getLocalBounds().removeFromTop(height);
    g.setColour(Colours::grey);
    g.setOpacity(1.f);
    g.drawRect(bounds);
    
    auto relativeHeight = jmap(peakLevel, (float) 0, (float) height);
    auto levelToFill = bounds.removeFromBottom(relativeHeight).reduced(5);
    
    FillType fill(*gradient);
    g.setFillType(fill);
    g.fillRect(levelToFill);
}

void LevelMeterComponent::resized()
{
    Point<float> start(0.f, getHeight() - 20);
    Point<float> end(getWidth(), 0.f);
    gradient = std::make_unique<ColourGradient>(Colours::black, start, Colours::black, end, true);
    gradient->clearColours();
    gradient->addColour(0.2, Colours::green);
    gradient->addColour(0.7, Colours::orange);
    gradient->addColour(1.f, Colours::red);
}

void LevelMeterComponent::timerCallback()
{
    getPeakValue();
    repaint();
}

void LevelMeterComponent::getPeakValue()
{
    if (analyser != nullptr)
    {
        if (analyser->nextFftBlockIsReady())
        {
            peakLevel = analyser->getPeakValue();
            analyser->setNextFftBlockIsReady(false);
        }
    }
}

void LevelMeterComponent::setAnalyser(Analyser* a)
{
    analyser = a;
}
