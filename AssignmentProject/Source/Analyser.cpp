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
        window(fftSize, dsp::WindowingFunction<float>::blackman),
        nextFftBlockReady(false)
{
    
}

void Analyser::pushSampleToQueue(float sample)
{
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    
    m.lock();
    
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
    
    m.unlock();
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

float Analyser::getPeakValue()
{
    
    for(int i = 0; i < fftSize; i++)
    {
        m.lock();
        aval = fabs(fifo[i]); //peak level detector
        m.unlock();
        
        if (aval > peakValue)
            peakValue = aval;
        
        measuredItems++;
        
        if (measuredItems == measuredLength)
        {
            newPeakValue = log10(peakValue * 39 + 1) / fLog40;
            peakValue = measuredItems = 0;
        }
        
        float coeff = (newPeakValue > oldPeakValue) ? 0.1 : 0.0001;
        level = coeff * newPeakValue + (1 - coeff) * oldPeakValue;
        oldPeakValue = level;
    }
    
    return level;
}
