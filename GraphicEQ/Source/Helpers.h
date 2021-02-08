/*
  ==============================================================================

    Helpers.h
    Created: 7 Feb 2021 8:48:55pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

class Helpers
{
public:

    /** Swaps two atomic pointers
     @param x   pointer one
     @param y   pointer two */
    template<typename Type>
    static void swapAtomicPointers(std::atomic<Type*>&x, std::atomic<Type*>&y)
    {
        Type* temp = x.load();
        x = y.load();
        y = temp;
    }
    
    static float frequencyToDecimal (float freq)
    {
        return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
    }

};
