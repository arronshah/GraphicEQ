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

/** A component for drawing the average filter response curve from an array of filter magnitude data.
 @see FilterResponseCurveComponent */
class AverageFilterResponseCurveComponent : public FilterResponseCurveComponent
{
public:
    /** Initialises the filter magnitude values */
    void initialise(const int numberOfFilters);
    
    /** Draws the average filter response curve of an array of filters */
    void drawResponseCurve(std::vector<double>* frequencies, std::vector<double>& mags) override;
    
    /** Sets the magnitude array for a given filter
     @param mags    specifies the new vector of magnitudes
     @param filterType  specifies the filter of which to replace the magnitudes for*/
    void setMagnitudes(std::vector<double>& mags, int filterType);
    
//    /** Sets the number of filter response curves this class will average over
//     @param numberOfFilters     specififes the number of filters*/
//    void setNumberOfFilters(int numberOfFilters);
    
    /** Resets the magnitude values currently stored for a given filter
     @param index   specifies the index of the filter to reset the magnitudes of*/
    void resetMagnitudes(int index);
    void paint(Graphics& g) override;
    
private:
    //std::array<std::vector<double>, 3> filterMagnitudes;
    std::unique_ptr<std::vector<std::vector<double>>> filterMagnitudes {nullptr};
    std::vector<double> totalMagnitudes;
    bool filterState;
};
