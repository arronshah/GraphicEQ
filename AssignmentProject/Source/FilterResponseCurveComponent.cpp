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
    responseCurve.clear();
}

void FilterResponseCurveComponent::paint(Graphics& g)
{
    g.setColour(Colours::grey);
    g.setOpacity(1.f);
    g.strokePath(responseCurve, PathStrokeType(3.f));
    
    Point<float> start(0.f, (float) getHeight() - 20);
    Point<float> end((float) getWidth(), (float) getHeight() - 20);
    ColourGradient gradient(Colours::grey, start, Colours::paleturquoise, end, true);
    FillType fill(gradient);
    fill.setOpacity(0.2);
    g.setFillType(fill);
    g.fillPath(responseCurve);
}

void FilterResponseCurveComponent::resized()
{
    
}

void FilterResponseCurveComponent::drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags)
{
    responseCurve.clear();
    
    float height = getHeight() - 20;
    float width = getWidth();
    
    responseCurve.startNewSubPath(0.f, height);
    
    float pixelsPerDouble = 2.f * height / Decibels::decibelsToGain(24.f);
    
    const double xFactor = static_cast<double> (width / frequencies->size());
    for (size_t i=1; i < frequencies->size(); ++i)
    {
        float x = float (getX() + i * xFactor);
        float yCalc = float (mags [i] > 0 ? (height / 2.f) - pixelsPerDouble * std::log (mags [i]) / std::log (2) : height);
        float y = (yCalc > height) ? height : yCalc;
        responseCurve.lineTo (x, y);
    }
    
    responseCurve.lineTo(width, height);
    responseCurve.closeSubPath();
    
    repaint();
}


