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
    node.setProperty("gain", 0.f, nullptr);
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
    
    if(frequency != prevFrequency)
    {
        smoothedFrequency.setTargetValue(frequency);
        prevFrequency = frequency;
    }
    
    if(filterType == 0){
        *filter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, smoothedFrequency.getNextValue(), resonance);
    }
    else if(filterType == 1){
        *filter.state = *dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, smoothedFrequency.getNextValue(), resonance);
    }
    else{
        *filter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, smoothedFrequency.getNextValue(), resonance);
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
