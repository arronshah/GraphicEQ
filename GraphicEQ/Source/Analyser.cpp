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
        window(fftSize, dsp::WindowingFunction<float>::hamming),
        nextFftBlockReady(false),
        buffer1(fftSize),
        buffer2(fftSize)
{
    writeBuffer = &buffer1;
    readBuffer = &buffer2;
}

void Analyser::pushSampleToBuffer(float sample)
{
    if (writeBuffer.load()->bufferIsFull())
    {
        if (! nextFftBlockReady)
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy(fftData, writeBuffer.load()->getBuffer().data(), writeBuffer.load()->getBufferSize());
            Helpers::swapAtomicPointers(writeBuffer, readBuffer);
            nextFftBlockReady = true;
        }
    }
    
    writeBuffer.load()->writeSample(sample);
}

float* Analyser::getFftData()
{
    window.multiplyWithWindowingTable (readBuffer.load()->getBuffer().data(), fftSize);
    fft.performFrequencyOnlyForwardTransform (fftData);
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
    float absoluteValue = 0;
    float measuredSamples = 0;
    int measuredLength = 44;
    const float fLog40 = log10(40);
    float oldPeakValue = 0;
    float peakValue = 0;
    float newPeakValue = 0;
    float level = 0;

    for(int i = 0; i < fftSize; i++)
    {
        absoluteValue = fabs(readBuffer.load()->getSample(i));
        
        if (absoluteValue > peakValue)
            peakValue = absoluteValue;
        
        measuredSamples++;
        
        if (measuredSamples == measuredLength)
        {
            newPeakValue = log10(peakValue * 39 + 1) / fLog40;
            peakValue = measuredSamples = 0;
        }
        
        float coeff = (newPeakValue > oldPeakValue) ? 0.1 : 0.0001; //filter coefficient for whether value is rising or falling
        level = coeff * newPeakValue + (1 - coeff) * oldPeakValue;
        oldPeakValue = level;
    }
    
    return level;
}
