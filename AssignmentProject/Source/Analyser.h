/*
  ==============================================================================

    Analyser.h
    Created: 21 Dec 2020 10:45:57am
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum
{
    fftOrder  = 11,             // [1]
    fftSize   = 1 << fftOrder,  // [2]
    windowSize = 256             // [3]
};

class Analyser
{
public:
    Analyser();
    void pushSampleToQueue(float sample);
    void prepareFftData();
    float* getFftData();
    bool nextFftBlockIsReady();
    void setNextFftBlockIsReady(bool flag);
    float* getFifoCopy();
    
private:
    dsp::FFT fft;
    dsp::WindowingFunction<float> window;
    //FixedQueue<float, fftSize> fifo;
    float fifo[fftSize];
    float fifoCopy[fftSize];
    float fftData[fftSize * 2];
    int fifoIndex = 0;
    bool nextFftBlockReady = false;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Analyser)
    
};
