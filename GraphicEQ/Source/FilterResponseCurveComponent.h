/*
  ==============================================================================

    FilterResponseCurveComponent.h
    Created: 22 Dec 2020 6:14:38pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Filter.h"

class FilterResponseCurveComponent : public Component
{
public:
    /** Constructor */
    FilterResponseCurveComponent();
    
    /** Destructor */
    ~FilterResponseCurveComponent() = default;
    
    /** Applies the filter response curve to a path object
     @param frequencies     array of frequencies for each point in the path
     @param mags    array of magnitudes for each point in the path
     @param state   the state of the filter*/
    void drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags, bool state);
    
    void setState(bool newState);
    
    //Component
    void paint(Graphics& g);
    void resized();
    
private:
    Path responseCurve;
    bool filterState = false;
};
