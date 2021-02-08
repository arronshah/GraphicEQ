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
#include "Buffer.cpp"
#include "Helpers.h"

enum
{
    fftOrder  = 11,             // [1]
    fftSize   = 1 << fftOrder,  // [2]
    windowSize = 128             // [3]
};

class Analyser
{
public:
    /** Constructor */
    Analyser();
    
    /** Adds a new sample to the buffer
     @param sample   sample to add */
    void pushSampleToBuffer(float sample);
    
    /**  Applies windowing function to the read buffer and then performs a frequency-only fft
     @return a pointer to the first element of the array of fft data */
    float* getFftData();
    
    /** Returns whether or not the next block of fft data is ready to be read
     @return a boolean */
    bool nextFftBlockIsReady();
    
    /** Sets whether or not the next block of fft data is ready to be read
     @param flag    a boolean */
    void setNextFftBlockIsReady(bool flag);
    
    /** Calculates the peak amplitude of the read buffer
     @return the peak amplitude of the read buffer (0 - 1)*/
    float getPeakValue();
    
private:
    
    dsp::FFT fft;
    dsp::WindowingFunction<float> window;
    float fftData[fftSize * 2];
    std::atomic<bool> nextFftBlockReady;
    Buffer<float> buffer1;
    Buffer<float> buffer2;
    std::atomic<Buffer<float>*> writeBuffer;
    std::atomic<Buffer<float>*> readBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Analyser)
    
};
