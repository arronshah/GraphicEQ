/*
  ==============================================================================

    Audio.cpp
    Created: 17 Dec 2020 2:48:33pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "Audio.h"

Audio::Audio()
{
    //set the filePlayer as the audio source
    audioSourcePlayer.setSource (&audioFilePlayer);
    audioFilePlayer.setAnalyser(&analyser);
    
    for(int i = 0; i < 3; i++)
        audioFilePlayer.setFilter(&filter[i], i);
    
    auto errorMessage = audioDeviceManager.initialiseWithDefaultDevices (1, 2);
    if ( ! errorMessage.isEmpty())
        DBG (errorMessage);
    audioDeviceManager.addAudioCallback (this);

}

Audio::~Audio()
{
    audioDeviceManager.removeAudioCallback(this);
}

void Audio::audioDeviceIOCallback (const float** inputChannelData,
                                   int numInputChannels,
                                   float** outputChannelData,
                                   int numOutputChannels,
                                   int numSamples)
{
    // get the audio from our file player - player puts samples in the output buffer
    audioSourcePlayer.audioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
    
    //All audio processing is done here
    const float* inL = inputChannelData[0];
    
    float *outL = outputChannelData[0];
    float *outR = outputChannelData[1];
    
    while(numSamples--)
    {
        float fileOutL = *outL;
        float fileOutR = *outR;
        
        *outL = fileOutL;
        *outR = fileOutR;
        
        inL++;
        outL++;
        outR++;
    }
}


void Audio::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
}

void Audio::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
}

AudioFilePlayer* Audio::getAudioFilePlayer()
{
    return &audioFilePlayer;
}

Analyser* Audio::getAnalyser()
{
    return &analyser;
}

Filter* Audio::getFilter(int index)
{
    return &filter[index];
}
