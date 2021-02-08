/*
  ==============================================================================

    LevelMeterComponent.h
    Created: 26 Dec 2020 3:00:15pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Analyser.h"

class LevelMeterComponent : public Component,
                            public Timer
{
public:
    /** Constructor */
    LevelMeterComponent();
    
    /** Destructor */
    ~LevelMeterComponent();
    
    /** Gets the peak amplitude value from an Analyser object*/
    void getPeakValue();
    
    /** Sets the analyser reference to an Analyser object
     @param a   specifies the adress of the analyser
     @see Analyser*/
    void setAnalyser(Analyser* a);
    
    //Component
    void paint(Graphics& g) override;
    void resized() override;
    
    //Timer
    void timerCallback() override;
    
private:
    float peakLevel;
    Analyser* analyser;
    
};
