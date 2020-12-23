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
    std::vector<double>* getFrequencies();
    std::vector<double>& getMagnitudes();
    
private:
    ValueTree parameterValueTree;
    //ValueTreeDebugListener debugListener;
    dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients <float>> lowPassFilter;;
    float lastSampleRate = 44100;
    
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
    
    float prevFrequency = 0;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFrequency;
};
