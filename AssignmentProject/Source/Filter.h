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
    Filter(filterType type);
    ~Filter() = default;
    void process(dsp::AudioBlock<float> buffer);
    void prepare(int samplesPerBlockExpected, double sampleRate);
    void updateParameters();
    void setState(bool newState);
    bool getCurrentState();
    std::vector<double>* getFrequencies();
    std::vector<double>& getMagnitudes();
    void setFilterMagnitudesReady(bool flag);
    bool getFilterMagnitudesReady();
    void setFrequency(float newFrequency);
    void setResonance(float newResonance);
    void setGain(float newGain);
    
private:
    const int filterType;
    //ValueTree& parameterValueTree = {nullptr};
    dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients <float>> filter;
    float lastSampleRate = 44100;
    
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
    
    std::atomic<bool> currentState;
    std::atomic<bool> filterMagnitudesReady;
    
    std::atomic<float> frequency;
    std::atomic<float> resonance;
    std::atomic<float> gain;
    
    float prevFrequency = 0;
    float prevResonance = 0;
    float prevGain = 0;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFrequency;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedResonance;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedGain;
};
