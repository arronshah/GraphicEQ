/*
  ==============================================================================

    Filter.h
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum filterType
{
    LowPass,
    BandPass,
    HighPass
};

class Filter
{
public:
    Filter(filterType type, ValueTree& tree);
    ~Filter() = default;
    ValueTree* getParameterValueTree();
    void process(dsp::AudioBlock<float> buffer);
    void prepare(int samplesPerBlockExpected, double sampleRate);
    void updateParameters();
    std::vector<double>* getFrequencies();
    std::vector<double>& getMagnitudes();
    
private:
    const int filterType;
    ValueTree& parameterValueTree;
    dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients <float>> filter;
    float lastSampleRate = 44100;
    
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
    
    
    
    float prevFrequency = 0;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFrequency;
};
