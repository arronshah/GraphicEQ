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
    if(valueTree != nullptr)
    {
        if(valueTree->getProperty("state"))
            paintIfFilterOn(g);
        else
            paintIfFilterOff(g);
    }
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

void FilterResponseCurveComponent::drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags)
{
    responseCurve.clear();
   
    float height = getHeight() - 20;
    float width = getWidth();
    
    responseCurve.startNewSubPath(0.f, height);
    
    //credit to ffAudio's Frequalizer for the response curve scaling formula below (lines 74-84) https://github.com/ffAudio/Frequalizer
    float pixelsPerDouble = 2.f * height / Decibels::decibelsToGain(24.f);
    
    const double xFactor = static_cast<double> (width / frequencies->size());
    
    for (size_t i=1; i < frequencies->size(); ++i)
    {
        float xPosition = float (getX() + i * xFactor);
        float yCalc = float (mags[i] > 0 ? (height / 2.f) - pixelsPerDouble * std::log (mags[i]) / std::log (2) : height);
        float yPosition = (yCalc > height) ? height : yCalc;
        responseCurve.lineTo (xPosition, yPosition);
    }

    float frequencyAsDecimal = Helpers::frequencyToDecimal(valueTree->getProperty("frequency"));
    float gain = valueTree->getProperty("gain");
    
    float xPos = jmap(frequencyAsDecimal, (float) 0.f, (float) getWidth());
    float yPos = jmap(gain, -24.f, 24.f, (float) getHeight() - 20 , 0.f);
    
    responseCurve.addEllipse(xPos, (float) yPos, 8.f, 8.f);
    
    repaint();
}


