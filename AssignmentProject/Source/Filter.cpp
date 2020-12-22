/*
  ==============================================================================

    Filter.cpp
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter()
    : parameterValueTree("filterValueTree"),
      debugListener(parameterValueTree, true)
{
    parameterValueTree.setProperty("frequency", 440.f, nullptr);
    parameterValueTree.setProperty("q-factor", 0.7, nullptr);
    parameterValueTree.setProperty("gain", 0.f, nullptr);
}

ValueTree* Filter::getParameterValueTree()
{
    return &parameterValueTree;
}

void Filter::process(dsp::AudioBlock<float> audioBlock)
{
    stateVariableFilter.process(dsp::ProcessContextReplacing<float>(audioBlock));
    updateParameters();
}

void Filter::prepare(int samplesPerBlockExpected, double sampleRate)
{
    dsp::ProcessSpec processSpec;
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlockExpected;
    processSpec.numChannels = 2;
    stateVariableFilter.prepare(processSpec);
    
    stateVariableFilter.setType(dsp::StateVariableTPTFilterType::lowpass);
    stateVariableFilter.setCutoffFrequency(parameterValueTree.getProperty("frequency"));
    stateVariableFilter.setResonance(0.7);
    
}

void Filter::updateParameters()
{
    stateVariableFilter.setCutoffFrequency(parameterValueTree.getProperty("frequency"));
}
