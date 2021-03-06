/*
  ==============================================================================

    Filter.h
    Created: 22 Dec 2020 2:39:54pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/** Specifies the filter type to be passed to a filter object
@see Filter */
enum filterType
{
    LowPass,
    BandPass,
    HighPass
};

/** A class for filtering a stream of audio sample data. */
/** Applies an inifite impulse response filter, which can be set to either low shelf, high shelf or peak
 @param type    an enum specifying the type of the filter.
 @see filterType*/
class Filter
{
public:
    /** Constructor */
    Filter(filterType type);
    
    /** Destructor */
    ~Filter() = default;
    
    /** Processes a block of samples
     @param buffer  buffer of samples to process*/
    void process(dsp::AudioBlock<float> buffer);
    
    /** Initialises the filter
     @param samplesPerBlockExpected   the size of the buffer to be processed
     @param sampleRate   the audio sample rate*/
    void prepare(int samplesPerBlockExpected, double sampleRate);
    
    /** Updates the frequency, resonance and gain parameters of the filters with linear smoothing*/
    void updateParameters();
    
    /** Sets the state of the filter to on or off
     @param newState    specifies the new state of the filter*/
    void setState(bool newState);
    
    /** Returns whether the filter is on or off
     @return a boolean*/
    bool getCurrentState();
    
    /** Returns an array of frequncies corresponding to each calculated filter magnitude
     @return an array of frequency values (20 - 20000 (Hz))
     @see getMagnitudes()*/
    std::vector<double>* getFrequencies();
    
    /** Returns an array of filter response curve magnitudes
     @return an array of filter magnitudes
     @see getFrequencies()*/
    std::vector<double>& getMagnitudes();
    
    /** Sets whether or not the next block of filter magnitudes is ready to be read
     @param a boolean*/
    void setFilterMagnitudesReady(bool flag);
    
    /** Returns whether or not the next block of filter magnitudes is ready to be read
     @return a boolean*/
    bool getFilterMagnitudesReady();
    
    /** Sets the frequency of the filter
     @param newFrequency    specifies the new filter frequency*/
    void setFrequency(float newFrequency);
    
    /** Sets the resonance of the filter
     @param newResonance    specifies the new filter resonance*/
    void setResonance(float newResonance);
    
    /** Sets the gain of the filter
     @param newGain    specifies the new filter gain*/
    void setGain(float newGain);
    
    /** Calculates the magnitudes for the frequency response of the filter */
    void calculateFilterMagnitudes();
    
    /** Returns true is a filter parameter value as changed
     @return parameterHasChanged    specifies whether or not a filter parameter has changed*/
    bool parameterHasChanged();
    
    /** Calculates the coefficients for the filter */
    void calculateFilterCoefficients();
    
    /** Applies linear smoothing to the filters parameter values */
    void smoothParameterValues();
    
private:
    const int filterType;
    dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients <float>> filter;
    float lastSampleRate = 44100;
    
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
    
    std::atomic<bool> currentState = 0;
    std::atomic<bool> filterMagnitudesReady;
    
    std::atomic<float> frequency;
    std::atomic<float> resonance;
    std::atomic<float> gain;
    
    float prevFrequency = 0;
    float prevResonance = 0;
    float prevGain = 0;
    bool prevState = 0;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFrequency;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedResonance;
    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedGain;
    
    std::atomic<bool> parameterChanged = false;
};
