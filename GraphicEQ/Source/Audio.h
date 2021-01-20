/*
  ==============================================================================

    Audio.h
    Created: 17 Dec 2020 2:48:33pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioFilePlayer.h"
#include "Analyser.h"
#include "Filter.h"

class Audio : public AudioIODeviceCallback
{
public:
    /** Constructor */
    Audio();
    
    /** Destructor */
    ~Audio();
    
    /** Returns the audio device manager*/
    AudioDeviceManager& getAudioDeviceManager() { return audioDeviceManager;}
    
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples) override;
    void audioDeviceAboutToStart (AudioIODevice* device) override;
    void audioDeviceStopped() override;
    AudioFilePlayer* getAudioFilePlayer();
    Analyser* getAnalyser();
    Filter* getFilter(int index);
private:
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioFilePlayer audioFilePlayer;
    Analyser analyser;
    Filter filter[3] = {{filterType::LowPass}, {filterType::BandPass}, {filterType::HighPass}};
};
