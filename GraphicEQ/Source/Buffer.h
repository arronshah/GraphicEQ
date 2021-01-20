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
    /** Constructor
     @param sizeInSamples   the size of the buffer to be created, in samples*/
    Buffer(int sizeInSamples);
    
    /** Destructor */
    ~Buffer() = default;
    
    /** Adds a new sample to the end of the buffer
     @param sample  specifies the new sample to add*/
    void writeSample(float sample);
    
    /** Gets the entire buffer contents
     @return a vector of samples*/
    std::vector<float> getBuffer();
    
    /** Clears the contents of the buffer */
    void clearBuffer();
    
    /** Gets the size of the buffer
     @return the buffer size as an integer*/
    int getBufferSize();
    
    /** Returns true if the buffer is full of samples and false if it is not
     @return a boolean*/
    bool bufferIsFull();
    
    /** Return the sample value at a given index in the buffer
     @return a floating point sample value (-1 - 1)*/
    float getSample(int index);
    
private:
    std::vector<float> buffer;
    int bufferIndex;
};
