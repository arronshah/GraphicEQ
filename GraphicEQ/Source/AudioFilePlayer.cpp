/*
  ==============================================================================

    AudioFilePlayer.cpp
    Created: 17 Dec 2020 2:55:46pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "AudioFilePlayer.h"

AudioFilePlayer::AudioFilePlayer() : thread("AudioFileThread")
{
    thread.startThread();
    
}

AudioFilePlayer::~AudioFilePlayer()
{
    audioTransportSource.setSource(nullptr);
    thread.stopThread(100);
}

void AudioFilePlayer::changeState(TransportState newAudioTransportState)
{
    if(newAudioTransportState != audioTransportState)
    {
        audioTransportState = newAudioTransportState;
        
        switch(audioTransportState)
        {
            case Play:
                audioTransportSource.start();
                break;
            case Pause:
                audioTransportSource.stop();
                break;
            case Stop:
                audioTransportSource.stop();
                audioTransportSource.setPosition(0.0);
                break;
        }
    }
}

bool AudioFilePlayer::isPlaying() const
{
    return audioTransportSource.isPlaying();
}

void AudioFilePlayer::loadFile (const File& newFile)
{
    changeState (Stop);
    audioTransportSource.setSource (nullptr);

    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    if (auto reader = formatManager.createReaderFor (newFile))
    {
        currentAudioFileSource = std::make_unique<AudioFormatReaderSource> (reader, true);
    
        audioTransportSource.setSource (currentAudioFileSource.get(),
                                        32768,
                                        &thread,
                                        reader->sampleRate);
    }
}

void AudioFilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioTransportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    
    for (int i = 0; i < 3; i++)
    {
        if(filter[i] != nullptr)
            filter[i]->prepare(samplesPerBlockExpected, sampleRate);
    }
}

void AudioFilePlayer::releaseResources()
{
    audioTransportSource.releaseResources();
}

void AudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioTransportSource.getNextAudioBlock (bufferToFill);
    
    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        dsp::AudioBlock<float> audioBlock(*bufferToFill.buffer);
        
        for (int i = 0; i < 3; i++)
        {
            if(filter[i]->getCurrentState())
                filter[i]->process(audioBlock);
        }
    }
    
    if(analyser != nullptr)
    {
        if (bufferToFill.buffer->getNumChannels() > 0)
        {
            auto* channelData = bufferToFill.buffer->getReadPointer (0, bufferToFill.startSample);
            
            for (auto i = 0; i < bufferToFill.numSamples; ++i)
                analyser->pushSampleToBuffer(channelData[i]);
        }
    }
}

double AudioFilePlayer::getCurrentPosition()
{
    return audioTransportSource.getCurrentPosition();
}

double AudioFilePlayer::getLengthOfFileInSeconds()
{
    return audioTransportSource.getLengthInSeconds();
}

void AudioFilePlayer::updatePosition(float newPosition)
{
    audioTransportSource.setPosition(newPosition);
}

void AudioFilePlayer::setAnalyser(Analyser* analyserRef)
{
    analyser = analyserRef;
}

void AudioFilePlayer::setFilter(Filter* filterRef, int index)
{
    filter[index] = filterRef;
}
