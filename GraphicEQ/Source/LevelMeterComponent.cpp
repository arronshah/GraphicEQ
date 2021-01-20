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
    g.setColour(Colours::darkturquoise);
    g.setOpacity(0.4);
    g.drawRect(bounds);
    
    auto relativeHeight = jmap(peakLevel, (float) 0, (float) height);
    auto levelToFill = bounds.removeFromBottom(relativeHeight).reduced(5);
    
    Point<float> start(0.f, height);
    Point<float> end(getWidth(), 0.f);
    ColourGradient gradient(Colours::darkturquoise, start, Colours::mediumpurple, end, true);
    FillType fill(gradient);
    fill.setOpacity(0.6);
    g.setFillType(fill);
    g.fillRect(levelToFill);
}

void LevelMeterComponent::resized()
{
    
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
