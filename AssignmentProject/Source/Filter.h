/*
  ==============================================================================

    Filter.h
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Filter
{
public:
    Filter();
    ~Filter() = default;
    ValueTree* getParameterValueTree();
    void process(dsp::AudioBlock<float> buffer);
    void prepare(int samplesPerBlockExpected, double sampleRate);
    void updateParameters();
    
private:
    ValueTree parameterValueTree;
    ValueTreeDebugListener debugListener;
    dsp::StateVariableTPTFilter<float> stateVariableFilter;
};
