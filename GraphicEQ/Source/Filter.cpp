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
}

void Filter::setFrequency(float newFrequency)
{
    frequency = jlimit(20.f, 20000.f, newFrequency);
}

void Filter::setResonance(float newResonance)
{
    resonance = jlimit((float) 0.1, (float) 18, newResonance);
}

void Filter::setGain(float newGain)
{
    gain = jlimit((float) 0.01, (float) 2, newGain);
}

void Filter::updateParameters()
{
    if(parameterHasChanged())
    {
        calculateFilterCoefficients();
        calculateFilterMagnitudes();
    }
}

void Filter::calculateFilterCoefficients()
{
    if(filterType == filterType::LowPass){
        *filter.state = *dsp::IIR::Coefficients<float>::makeLowShelf(lastSampleRate, smoothedFrequency.getNextValue(), smoothedResonance.getNextValue(), smoothedGain.getNextValue());
    }
    else if(filterType == filterType::BandPass){
        *filter.state = *dsp::IIR::Coefficients<float>::makePeakFilter(lastSampleRate, smoothedFrequency.getNextValue(), smoothedResonance.getNextValue(), smoothedGain.getNextValue());
    }
    else if(filterType == filterType::HighPass){
        *filter.state = *dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, smoothedFrequency.getNextValue(), smoothedResonance.getNextValue(), smoothedGain.getNextValue());
    }
}

bool Filter::parameterHasChanged()
{
    bool parameterChanged = false;
    
    if(frequency.load() != prevFrequency)
    {
        smoothedFrequency.setTargetValue(frequency.load());
        prevFrequency = frequency.load();
        parameterChanged = true;
    }
    if(resonance.load() != prevResonance)
    {
        smoothedResonance.setTargetValue(resonance.load());
        prevResonance = resonance.load();
        parameterChanged = true;
    }
    if(gain.load() != prevGain)
    {
        smoothedGain.setTargetValue(gain.load());
        prevGain = gain.load();
        parameterChanged = true;
    }
    
    return parameterChanged;
}

void Filter::calculateFilterMagnitudes()
{
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
