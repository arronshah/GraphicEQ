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
    if (analyser != nullptr)
    {
        if (analyser->nextFftBlockIsReady())
        {
            fifoCopy = analyser->getFifoCopy();
            process();
        }
        
        repaint();
    }
}

void LevelMeterComponent::process()
{
    for(int i = 0; i < fftSize; i++)
    {
        aval = fabs(fifoCopy[i]); //peak level detector
        
        if (aval > max)
            max = aval;
        
        measuredItems++;
        
        if (measuredItems == measuredLength)
        {
            newMax = log10(max * 39 + 1) / fLog40;
            max = measuredItems = 0;
        }
        
        float coeff = (newMax > oldMax) ? 0.1 : 0.0001;
        peakLevel = coeff * newMax + (1 - coeff) * oldMax;
        oldMax = peakLevel;
    }
}

void LevelMeterComponent::setAnalyser(Analyser* a)
{
    analyser = a;
}
