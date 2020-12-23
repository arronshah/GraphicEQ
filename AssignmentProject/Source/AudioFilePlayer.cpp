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
    audioTransportSource.setSource(nullptr); //unload file
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
    // unload the previous file source and delete it..
    changeState (Stop);
    audioTransportSource.setSource (nullptr);
    
    // create a new file source from the file..
    // get a format manager and set it up with the basic types (wav, ogg and aiff).
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    if (auto reader = formatManager.createReaderFor (newFile))
    {
        //currentFile = audioFile;
        currentAudioFileSource = std::make_unique<AudioFormatReaderSource> (reader, true);
        
        // ..and plug it into our transport source
        audioTransportSource.setSource (currentAudioFileSource.get(),
                                        32768, // tells it to buffer this many samples ahead
                                        &thread,
                                        reader->sampleRate);
    }
}

//AudioSource functions to route through to AudioTransportSource
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
    
    if(filter[2] != nullptr)
    {
        if (bufferToFill.buffer->getNumChannels() > 0)
        {
            dsp::AudioBlock<float> audioBlock(*bufferToFill.buffer);
            
            for (int i = 0; i < 3; i++)
            {
                if(filter[i]->getCurrentState())
                    filter[i]->process(audioBlock);
            }
            
        }
    }
    
    if(analyser != nullptr)
    {
        if (bufferToFill.buffer->getNumChannels() > 0)
        {
            auto* channelData = bufferToFill.buffer->getReadPointer (0, bufferToFill.startSample);
            
            for (auto i = 0; i < bufferToFill.numSamples; ++i)
                analyser->pushSampleToQueue (channelData[i]);
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
