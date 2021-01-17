/*
  ==============================================================================

    Buffer.h
    Created: 17 Jan 2021 6:10:18pm
    Author:  Arron Shah

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

class Buffer
{
public:
    Buffer(int sizeInSamples);
    ~Buffer() = default;
    void writeSample(float sample);
    std::vector<float> getBuffer();
    void clearBuffer();
    int getBufferSize();
    bool bufferIsFull();
    float getSample(int index);
    
private:
    std::vector<float> buffer;
    int bufferIndex;
};
