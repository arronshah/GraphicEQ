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
    initialise();
}

void FilterResponseCurveComponent::paint(Graphics& g)
{
    if(valueTree != nullptr)
    {
        if(valueTree->getProperty("state"))
            paintIfFilterOn(g);
        else
            paintIfFilterOff(g);
    }
}

void FilterResponseCurveComponent::initialise()
{
    responseCurve.clear();
}

void FilterResponseCurveComponent::setValueTree(ValueTree* vt)
{
    valueTree = vt;
}

void FilterResponseCurveComponent::setColour(Colour newColour)
{
    colour = newColour;
}

void FilterResponseCurveComponent::paintIfFilterOn(Graphics& g)
{
    g.setColour(colour);
    g.setOpacity(1.f);
    g.strokePath(responseCurve, PathStrokeType(3.f));
    g.strokePath(responseCurveHandle, PathStrokeType(3.f));
    g.fillPath(responseCurveHandle);
}

void FilterResponseCurveComponent::paintIfFilterOff(Graphics& g)
{
    g.setColour(Colours::grey);
    g.setOpacity(1.f);
    g.strokePath(responseCurve, PathStrokeType(3.f));
}

void FilterResponseCurveComponent::resized()
{
    
}

void FilterResponseCurveComponent::drawResponseCurveHandle(float filterFrequency, float filterGain)
{
    responseCurveHandle.clear();
    
    float freqAsDecimal = Helpers::frequencyToDecimal(filterFrequency);
    
    float xPos = jmap(freqAsDecimal, (float) 0.f, (float) getWidth());
    float yPos = jmap(filterGain, -24.f, 24.f, (float) getHeight() - 20 , 0.f);
    
    responseCurveHandle.addEllipse(xPos, (float) yPos, 8.f, 8.f);
    
    repaint();
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
        float yCalc = float (mags[i] > 0 ? (height / 2.f) - pixelsPerDouble * std::log (mags[i]) / std::log (2) : height);
        float y = (yCalc > height) ? height : yCalc;
        responseCurve.lineTo (x, y);
    }
    
    repaint();
}


