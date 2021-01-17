/*
  ==============================================================================

    Analyser.h
    Created: 21 Dec 2020 10:45:57am
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Buffer.h"

enum
{
    fftOrder  = 11,             // [1]
    fftSize   = 1 << fftOrder,  // [2]
    windowSize = 128             // [3]
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
    float getPeakValue();
    
private:
    dsp::FFT fft;
    dsp::WindowingFunction<float> window;
    float fifo[fftSize];
    float fftData[fftSize * 2];
    int fifoIndex = 0;
    std::atomic<bool> nextFftBlockReady;
    
    Buffer buffer1;
    Buffer buffer2;
    
    std::atomic<Buffer*> writeBuffer;
    std::atomic<Buffer*> readBuffer;
    
    float aval = 0;
    float measuredItems = 0;
    int measuredLength = 44;
    const float fLog40 = log10(40);
    float oldPeakValue = 0;
    float peakValue = 0;
    float newPeakValue = 0;
    float level = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Analyser)
    
};
