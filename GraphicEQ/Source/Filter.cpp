/*
  ==============================================================================

    Filter.cpp
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter(enum filterType type)
    : filterType(type),
      currentState(false),
      filterMagnitudesReady(false),
      frequency(440.f),
      resonance(0.7),
      gain(1.0)
{
    frequencies.resize (300);
    for (size_t i=0; i < frequencies.size(); i++)
    {
        frequencies [i] = 20.0 * std::pow (2.0, i / 30.0);
    }
    magnitudes.resize (frequencies.size());
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

void Filter::setFrequency(float newFrequency)
{
    frequency = newFrequency;
}

void Filter::setResonance(float newResonance)
{
    resonance = newResonance;
}

void Filter::setGain(float newGain)
{
    gain = newGain;
}

void Filter::updateParameters()
{
    
    if(frequency.load() != prevFrequency)
    {
        smoothedFrequency.setTargetValue(frequency.load());
        prevFrequency = frequency.load();
    }
    if(resonance.load() != prevResonance)
    {
        smoothedResonance.setTargetValue(resonance.load());
        prevResonance = resonance.load();
    }
    if(gain.load() != prevGain)
    {
        smoothedGain.setTargetValue(gain.load());
        prevGain = gain.load();
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
    
    if(!filterMagnitudesReady.load())
    {
        filter.state->getMagnitudeForFrequencyArray(frequencies.data(), magnitudes.data(), frequencies.size(), lastSampleRate);
        filterMagnitudesReady = true;
    }
}

void Filter::setFilterMagnitudesReady(bool flag)
{
    filterMagnitudesReady = flag;
}

bool Filter::getFilterMagnitudesReady()
{
    return filterMagnitudesReady.load();
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
    return currentState.load();
}

void Filter::setState(bool newState)
{
    currentState = newState;
}
