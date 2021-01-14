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
    float peakLevel;
    Analyser* analyser;
};
