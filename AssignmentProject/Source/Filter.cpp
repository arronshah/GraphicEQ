/*
  ==============================================================================

    Filter.cpp
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter(enum filterType type, ValueTree& tree)
    : filterType(type),
      parameterValueTree(tree)
{
    String filterParams;
    filterParams << "filterParams" << filterType;
    ValueTree node(filterParams);
    node.setProperty("filterType", filterType, nullptr);
    node.setProperty("frequency", 440.f, nullptr);
    node.setProperty("resonance", 0.7, nullptr);
    node.setProperty("gain", 1.f, nullptr);
    parameterValueTree.addChild(node, filterType, nullptr);
    
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
    filter.process(dsp::ProcessContextReplacing<float>(audioBlock));
}

void Filter::prepare(int samplesPerBlockExpected, double sampleRate)
{
    lastSampleRate = sampleRate;
    
    dsp::ProcessSpec processSpec;
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlockExpected;
    processSpec.numChannels = 2;
    
    filter.prepare(processSpec);
    filter.reset();
    updateParameters();
    
}

void Filter::updateParameters()
{
    ValueTree tree = parameterValueTree.getChild(filterType);
    float frequency = tree.getProperty("frequency");
    float resonance = tree.getProperty("resonance");
    float gain = tree.getProperty("gain");
    
    if(frequency != prevFrequency)
    {
        smoothedFrequency.setTargetValue(frequency);
        prevFrequency = frequency;
    }
    if(resonance != prevResonance)
    {
        smoothedResonance.setTargetValue(resonance);
        prevFrequency = frequency;
    }
    if(gain != prevGain)
    {
        smoothedGain.setTargetValue(gain);
        prevFrequency = frequency;
    }
    
    if(filterType == 0){
        *filter.state = *dsp::IIR::Coefficients<float>::makeLowShelf(lastSampleRate, smoothedFrequency.getNextValue(), smoothedResonance.getNextValue(), smoothedGain.getNextValue());
    }
    else if(filterType == 1){
        *filter.state = *dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, smoothedFrequency.getNextValue(), smoothedResonance.getNextValue(), smoothedGain.getNextValue());
    }
    else{
        *filter.state = *dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, smoothedFrequency.getNextValue(), smoothedResonance.getNextValue(), smoothedGain.getNextValue());
    }
    
    filter.state->getMagnitudeForFrequencyArray(frequencies.data(), magnitudes.data(), frequencies.size(), lastSampleRate);
    
}

std::vector<double>* Filter::getFrequencies()
{
    return &frequencies;
}
std::vector<double>& Filter::getMagnitudes()
{
    return magnitudes;
}

bool Filter::getCurrentState()
{
    return currentState;
}

void Filter::setState(bool newState)
{
    currentState = newState;
}
