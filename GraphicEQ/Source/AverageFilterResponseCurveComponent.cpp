/*
  ==============================================================================

    AverageFilterResponseCurveComponent.cpp
    Created: 7 Feb 2021 1:07:15pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "AverageFilterResponseCurveComponent.h"

void AverageFilterResponseCurveComponent::initialise()
{
    responseCurve.clear();
    
    for(int i = 0; i < filterMagnitudes.size(); i++)
    {
        filterMagnitudes[i].resize(300);
        std::fill(filterMagnitudes[i].begin(), filterMagnitudes[i].end(), 1.f);
    }
    
    totalMagnitudes.resize(300);
    
    filterState = true;
}

void AverageFilterResponseCurveComponent::setMagnitudes(std::vector<double>& mags, int filterType)
{
    filterMagnitudes[filterType] = mags;
}

void AverageFilterResponseCurveComponent::paint(Graphics& g)
{
    paintIfFilterOn(g);
}

void AverageFilterResponseCurveComponent::resetMagnitudes(int index)
{
    std::fill(filterMagnitudes[index].begin(), filterMagnitudes[index].end(), 1.f);
}

void AverageFilterResponseCurveComponent::drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags)
{
    std::fill (totalMagnitudes.begin(), totalMagnitudes.end(), 1.f);
    
    for(int i = 0; i < filterMagnitudes.size(); i++)
        FloatVectorOperations::multiply(totalMagnitudes.data(), filterMagnitudes[i].data(), static_cast<int>(totalMagnitudes.size()));
    
    responseCurve.clear();

    float height = getHeight() - 20;
    float width = getWidth();
    
    responseCurve.startNewSubPath(0.f, height);
    
    float pixelsPerDouble = 2.f * height / Decibels::decibelsToGain(24.f);
    
    const double xFactor = static_cast<double> (width / frequencies->size());
    
    for (size_t i=1; i < frequencies->size(); ++i)
    {
        float x = float (getX() + i * xFactor);
        float yCalc = float (totalMagnitudes[i] > 0 ? (height / 2.f) - pixelsPerDouble * std::log (totalMagnitudes[i]) / std::log (2) : height);
        float y = (yCalc > height) ? height : yCalc;
        responseCurve.lineTo (x, y);
    }
    
    //responseCurve.lineTo(width, height);
    //responseCurve.closeSubPath();
    
    repaint();
}
