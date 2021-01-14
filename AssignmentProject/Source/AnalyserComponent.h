/*
  ==============================================================================

    AnalyserComponent.h
    Created: 21 Dec 2020 11:14:09am
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Analyser.h"
#include <array>
#include <cmath>

class AnalyserComponent : public Component,
                          public Timer
{
public:
    AnalyserComponent();
    ~AnalyserComponent() = default;
    void paint(Graphics&) override;
    void timerCallback() override;
    void resized() override;
    void drawNextFrame();
    void setAnalyser(Analyser* analyserRef);
    void drawPath(Graphics& g);
    void drawGrid(Graphics& g);
    float frequencyToProportion (float freq);
    void paintOverChildren(Graphics& g) override;
    float spectrumGainToProportion (float gain);
    Point<float> getPointFromIndex(int index);
    
private:
    Analyser* analyser {nullptr};
    float* fftData {nullptr};
    float windowData[windowSize];
    Path path;
    bool gridIsDrawn = false;
    float gridFrequencyValues[28] = {
        20.f, 30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f, 100.f, 200.f, 300.f, 400.f, 500.f, 600.f, 700.f, 800.f, 900.f, 1000.f, 2000.f, 3000.f, 4000.f, 5000.f, 6000.f, 7000.f, 8000.f, 9000.f, 10000.f, 20000.f
    };
    
    float gridFrequencyLabels[9] = {20.f, 50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f};
    
    float gridGainValues[7] = {0.f, -12.f, -24.f, -36.f, -48.f, -60.f, -72.f};
    
    float peakLevel = 0;
    
    std::mutex m;
    
    
};
