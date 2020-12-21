/*
  ==============================================================================

    Analyser.cpp
    Created: 21 Dec 2020 10:45:57am
    Author:  Arron Shah

  ==============================================================================
*/

#include "Analyser.h"

Analyser::Analyser() :
        fft(fftOrder),
        window(fftSize, dsp::WindowingFunction<float>::blackman)
{
    
}

void Analyser::pushSampleToQueue(float sample)
{
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    if (fifoIndex == fftSize)               // [11]
    {
        if (! nextFftBlockReady)            // [12]
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFftBlockReady = true;
        }
        
        fifoIndex = 0;
    }
    
    fifo[fifoIndex++] = sample;             // [12]
}

float* Analyser::getFftData()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable (fifo, fftSize);       // [1]
    
    // then render our FFT data..
    fft.performFrequencyOnlyForwardTransform (fftData);  // [2]
    
    return fftData;
}

bool Analyser::nextFftBlockIsReady()
{
    return nextFftBlockReady;
}

void Analyser::setNextFftBlockIsReady(bool flag)
{
    nextFftBlockReady = flag;
}
