/*
  ==============================================================================

    Buffer.cpp
    Created: 17 Jan 2021 6:10:18pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Buffer.h"

template<class Type>
Buffer<Type>::Buffer(int sizeInSamples)
{
    buffer.resize(sizeInSamples);
    bufferIndex = 0;
}

template<class Type>
void Buffer<Type>::writeSample(Type sample)
{
    buffer[bufferIndex++] = sample;
}

template<class Type>
std::vector<Type> Buffer<Type>::getBuffer()
{
    return buffer;
}

template<class Type>
void Buffer<Type>::clearBuffer()
{
    buffer.clear();
}

template<class Type>
int Buffer<Type>::getBufferSize()
{
    return static_cast<int>(buffer.size());
}

template<class Type>
bool Buffer<Type>::bufferIsFull()
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

template<class Type>
Type Buffer<Type>::getSample(int index)
{
    return buffer[index];
}

