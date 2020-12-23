/*
  ==============================================================================

    Filter.cpp
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter()
    : parameterValueTree("filterValueTree")
      //debugListener(parameterValueTree, true)
{
    parameterValueTree.setProperty("frequency", 440.f, nullptr);
    parameterValueTree.setProperty("resonance", 0.7, nullptr);
    parameterValueTree.setProperty("gain", 0.f, nullptr);
    
    frequencies.resize (300);
    for (size_t i=0; i < frequencies.size(); ++i) {
        frequencies [i] = 20.0 * std::pow (2.0, i / 30.0);
    }
    magnitudes.resize (frequencies.size());
    
    smoothedFrequency.reset(lastSampleRate, 0.0001);
    smoothedFrequency.setCurrentAndTargetValue(parameterValueTree.getProperty("frequency"));
}

ValueTree* Filter::getParameterValueTree()
{
    return &parameterValueTree;
}

void Filter::process(dsp::AudioBlock<float> audioBlock)
{
    updateParameters();
    lowPassFilter.process(dsp::ProcessContextReplacing<float>(audioBlock));
}

void Filter::prepare(int samplesPerBlockExpected, double sampleRate)
{
    lastSampleRate = sampleRate;
    
    dsp::ProcessSpec processSpec;
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlockExpected;
    processSpec.numChannels = 2;
    
    lowPassFilter.prepare(processSpec);
    lowPassFilter.reset();
    updateParameters();
    
}

void Filter::updateParameters()
{
    float frequency = parameterValueTree.getProperty("frequency");
    float resonance = parameterValueTree.getProperty("resonance");
    
    if(frequency != prevFrequency)
    {
        smoothedFrequency.setTargetValue(frequency);
        prevFrequency = frequency;
    }
    
    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, smoothedFrequency.getNextValue(), resonance);
    lowPassFilter.state->getMagnitudeForFrequencyArray(frequencies.data(), magnitudes.data(), frequencies.size(), lastSampleRate);
    
}

std::vector<double>* Filter::getFrequencies()
{
    return &frequencies;
}
std::vector<double>& Filter::getMagnitudes()
{
    return magnitudes;
}
