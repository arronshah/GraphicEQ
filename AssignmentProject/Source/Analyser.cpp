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
        nextFftBlockReady(false),
        buffer1(fftSize),
        buffer2(fftSize)
{
    writeBuffer = &buffer1;
    readBuffer = &buffer2;
}

void Analyser::pushSampleToQueue(float sample)
{
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    if (writeBuffer.load()->bufferIsFull())               // [11]
    {
        if (! nextFftBlockReady)            // [12]
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy(fftData, writeBuffer.load()->getBuffer().data(), writeBuffer.load()->getBufferSize());
            
            Buffer* tempPointer = writeBuffer.load();
            writeBuffer = readBuffer.load();
            readBuffer = tempPointer;
            
            nextFftBlockReady = true;
        }
    }
    
    writeBuffer.load()->writeSample(sample);           // [12]
}

float* Analyser::getFftData()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable (readBuffer.load()->getBuffer().data(), fftSize);       // [1]
    
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
