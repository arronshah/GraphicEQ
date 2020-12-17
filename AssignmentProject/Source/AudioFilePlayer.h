/*
  ==============================================================================

    AudioFilePlayer.h
    Created: 17 Dec 2020 2:55:46pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioFilePlayer : public AudioSource
{
public:
    /** Constructor */
    AudioFilePlayer();
    
    /** Destructor */
    ~AudioFilePlayer();
    
    /** Starts or stops playback of the file */
    void setPlaying (bool newState);
    
    /** Gets the current playback state of the file */
    bool isPlaying() const;
    
    /** Loads the specified file into the transport source */
    void loadFile (const File& newFile);
    
    //AudioSource
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
private:
    std::unique_ptr<AudioFormatReaderSource> currentAudioFileSource;    //reads audio from the file
    AudioTransportSource audioTransportSource;            // this controls the playback of a positionable audio stream, handling the starting/stopping and sample-rate conversion
    TimeSliceThread thread;
};
