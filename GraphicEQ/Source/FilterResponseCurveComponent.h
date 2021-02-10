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

/** A component class for drawing a filter response curve to the GUI from a set of filter magnitude data.
 @see Filter*/
class FilterResponseCurveComponent : public Component
{
public:
    /** Constructor */
    FilterResponseCurveComponent();
    
    /** Destructor */
    ~FilterResponseCurveComponent() = default;
    
    /** Draws the filter response curve to a path object
     @param frequencies     array of frequencies for each point in the path
     @param mags    array of magnitudes for each point in the path*/
    virtual void drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags);
    
    /** Paints the curve in colour to indicate the filter being on
     @param g   a reference to a graphics object
     @see paintIfFilterOff()*/
    void paintIfFilterOn(Graphics& g);
    
    /** Paints the curve in greyscale to indicate the filter being off
     @param g   a reference to a graphics object
     @see paintIfFilterOn()*/
    void paintIfFilterOff(Graphics& g);
    
    /** Sets the colour of the curve
     @param newColour   specifies the new colour to be assigned to the curve*/
    void setColour(Colour newColour);
    
    /** Sets the value tree object which holds the state of the filter
     @param vt  a pointer to a ValueTree object*/
    void setValueTree(ValueTree* vt);
    
    //Component
    void paint(Graphics& g);
    void resized();
    
protected:
    Path responseCurve;
    int filterType;
    Colour colour = Colours::lightgrey;
    ValueTree* valueTree {nullptr};
};
