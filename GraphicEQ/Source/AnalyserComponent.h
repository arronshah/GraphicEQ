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
#include "Helpers.h"
#include <array>
#include <cmath>

/** A component class for visualising the FFT data recieved from an Analyser object on the UI
 @see Analyser*/
class AnalyserComponent : public Component,
                          public Timer
{
public:
    /** Contructor */
    AnalyserComponent();
    
    /** Destructor */
    ~AnalyserComponent() = default;
    
    //Component
    void paint(Graphics&) override;
    void resized() override;
    
    //Timer
    void timerCallback() override;
    
    /** Prepare the next frame of spectrum data to be drawn*/
    void prepareNextFrame();
    
    /** Sets the analyser than this component receives data from
     @param analyserRef     a pointer to an Analyser object*/
    void setAnalyser(Analyser* analyserRef);
    
    /** Draws the frequency spectrum pathn
     @param g   a reference to a graphics object*/
    void drawPath(Graphics& g);
    
    /** Draws a logarithmic grid that sits behind the spectrum plot and caches it to an Image object
     @param g   a reference to a graphics object
     @see drawPath()*/
    void cacheGrid();
    
    /** Scales an amplitude value to the appropriate point on this components UI.
     @param windowDataIndex    the index of the windowData
     @return a Point object containing X and Y coordinates*/
    Point<float> getScaledPoint(int windowDataIndex);
    
    void pushPointsToOpenGLContext();
    
private:
    Analyser* analyser {nullptr};
    float* fftData {nullptr};
    float windowData[windowSize];
    Path path;
    bool gridIsCached = false;
    float gridFrequencyValues[28] = {
        20.f, 30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f, 100.f, 200.f, 300.f, 400.f, 500.f, 600.f, 700.f, 800.f, 900.f, 1000.f, 2000.f, 3000.f, 4000.f, 5000.f, 6000.f, 7000.f, 8000.f, 9000.f, 10000.f, 20000.f
    };
    
    float gridFrequencyLabels[9] = {20.f, 50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f};
    
    float gridGainValues[7] = {0.f, -12.f, -24.f, -36.f, -48.f, -60.f, -72.f};
    
    float peakLevel = 0;
    
    std::unique_ptr<Image> grid;
    
    std::array<float, windowSize> oldLevel;
    
    
};
