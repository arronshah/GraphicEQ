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
#include "Helpers.h"

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
    virtual void drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags);
    
    void drawResponseCurveHandle(float filterFrequency, float filterGain);
    
    virtual void initialise();
    
    void paintIfFilterOn(Graphics& g);
    
    void paintIfFilterOff(Graphics& g);
    
    void setColour(Colour newColour);
    
    void setValueTree(ValueTree* vt);
    
    //Component
    void paint(Graphics& g);
    void resized();
    
protected:
    Path responseCurve;
    int filterType;
    Colour colour = Colours::lightgrey;
    Path responseCurveHandle;
    ValueTree* valueTree {nullptr};
};
