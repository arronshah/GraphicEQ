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
    
    /** Returns the audio device manager, don't keep a copy of it! */
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
    ValueTree* getFilterValueTree();
    void setAudioVisualiserComponent(AudioVisualiserComponent* visualiser);
private:
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioFilePlayer audioFilePlayer;
    Analyser analyser;
    Filter filter;
};
