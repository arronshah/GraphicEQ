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
    LevelMeterComponent();
    ~LevelMeterComponent();
    void paint(Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    void process();
    void setAnalyser(Analyser* a);
private:
    float audioBlock[512];
    float outputLevel;
    float* fifoCopy {nullptr};
    float peakLevel;
    
    float aval = 0;
    float max = 0;
    float newMax = 0;
    float oldMax = 0;
    float measuredItems = 0;
    int measuredLength = 44;
    const float fLog40 = log10(40);
    
    Analyser* analyser;
};
