/*
  ==============================================================================

    Buffer.cpp
    Created: 17 Jan 2021 6:10:18pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Buffer.h"

Buffer::Buffer(int sizeInSamples)
{
    buffer.resize(sizeInSamples);
    bufferIndex = 0;
}

void Buffer::writeSample(float sample)
{
    buffer[bufferIndex++] = sample;
}

std::vector<float> Buffer::getBuffer()
{
    return buffer;
}

void Buffer::clearBuffer()
{
    buffer.clear();
}

int Buffer::getBufferSize()
{
    return static_cast<int>(buffer.size());
}

bool Buffer::bufferIsFull()
{
    if(bufferIndex == buffer.size())
    {
        bufferIndex = 0;
        return true;
    }
    else
    {
        return false;
    }
}

float Buffer::getSample(int index)
{
    return buffer[index];
}

