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
            swapAtomicPointers(writeBuffer, readBuffer);
            nextFftBlockReady = true;
        }
    }
    
    writeBuffer.load()->writeSample(sample);
}

template<typename Type> void Analyser::swapAtomicPointers(std::atomic<Type*>&x, std::atomic<Type*>&y)
{
    Type* temp = x.load();
    x = y.load();
    y = temp;
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
    float aval = 0;
    float measuredItems = 0;
    int measuredLength = 44;
    const float fLog40 = log10(40);
    float oldPeakValue = 0;
    float peakValue = 0;
    float newPeakValue = 0;
    float level = 0;

    for(int i = 0; i < fftSize; i++)
    {
        aval = fabs(readBuffer.load()->getSample(i)); //peak level detector
        
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
