/*
  ==============================================================================

    AverageFilterResponseCurveComponent.h
    Created: 7 Feb 2021 1:07:15pm
    Author:  Arron Shah

  ==============================================================================
*/
#include <JuceHeader.h>
#include "FilterResponseCurveComponent.h"
#pragma once

class AverageFilterResponseCurveComponent : public FilterResponseCurveComponent
{
public:
    void initialise() override;
    void drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags) override;
    void setMagnitudes(std::vector<double>& mags, int filterType);
    void resetMagnitudes(int index);
    void paint(Graphics& g) override;
    
private:
    std::array<std::vector<double>, 3> filterMagnitudes;
    std::vector<double> totalMagnitudes;
    bool filterState;
};
