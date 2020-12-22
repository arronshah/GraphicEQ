/*
  ==============================================================================

    FilterResponseCurveComponent.cpp
    Created: 22 Dec 2020 6:14:38pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "FilterResponseCurveComponent.h"

FilterResponseCurveComponent::FilterResponseCurveComponent()
{
    
}

void FilterResponseCurveComponent::paint(Graphics& g)
{
    responseCurve.clear();
    drawResponseCurve();
    g.setColour(Colours::grey);
    g.setOpacity(1.f);
    g.strokePath(responseCurve, PathStrokeType(3.f));
}

void FilterResponseCurveComponent::setFilter(Filter* filterRef)
{
    filter = filterRef;
}

void FilterResponseCurveComponent::resized()
{
    
}

float frequencyToProportion(float frequency)
{
    return (std::log (frequency / 20.0f) / std::log (2.0f)) / 10.0f;
}

void FilterResponseCurveComponent::drawResponseCurve()
{
    responseCurve.startNewSubPath(0.f, getHeight() - 20);
    
    std::vector<double>* frequencies = filter->getFrequencies();
    std::vector<double>& mags = filter->getMagnitudes();
    
    float pixelsPerDouble = 2.f * getHeight() / Decibels::decibelsToGain(24.f);
    
    const double xFactor = static_cast<double> (getWidth()) / frequencies->size();
    for (size_t i=1; i < frequencies->size(); ++i)
    {
        responseCurve.lineTo (float (getX() + i * xFactor),
                  float (mags [i] > 0 ? (getHeight() / 2.f) - pixelsPerDouble * std::log (mags [i]) / std::log (2) : getBottom()));
    }
    
    responseCurve.lineTo(getWidth(), getHeight() - 20);
    responseCurve.closeSubPath();
}

void FilterResponseCurveComponent::updatePath()
{
    if (filter != nullptr)
        repaint();  
}


