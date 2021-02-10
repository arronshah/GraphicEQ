/*
  ==============================================================================

    AudioFilePlayer.h
    Created: 17 Dec 2020 2:55:46pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Analyser.h"
#include "Filter.h"
#include "LevelMeterComponent.h"

#define NUMBER_OF_FILTERS 3

enum TransportState
{
    Play,
    Pause,
    Stop
};

/** A class for reading audio from a file and handling the playback stream */
class AudioFilePlayer : public AudioSource
{
public:
    /** Constructor */
    AudioFilePlayer();
    
    /** Destructor */
    ~AudioFilePlayer();
    
    /** Starts or stops playback of the file */
    void changeState (TransportState newAudioTransportState);
    
    /** Gets the current playback state of the file */
    bool isPlaying() const;
    
    /** Loads the specified file into the transport source */
    void loadFile (const File& newFile);
    
    //AudioSource
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    /** Return the current position of the audio stream*/
    double getCurrentPosition();
    
    /** Updates the position of the audio stream to the position specified
     @param newPosition     specifies the new position of the stream*/
    void updatePosition(float newPosition);
    
    /** Returns the length of the loaded audio file in seconds*/
    double getLengthOfFileInSeconds();
    
    /** Sets the audio analyser
     @param analyserRef     a reference to an audio analyser object
     @see Analyser*/
    void setAnalyser(Analyser* analyserRef);
    
    /** Sets the audio filter at a specified index
     @param filterRef     a reference to a filter object
     @param index         the array index of the filter object
     @see Filter*/
    void setFilter(Filter* filterRef, int index);
    
    
private:
    std::unique_ptr<AudioFormatReaderSource> currentAudioFileSource;    //reads audio from the file
    AudioTransportSource audioTransportSource;            // this controls the playback of a positionable audio stream, handling the starting/stopping and sample-rate conversion
    TimeSliceThread thread;
    TransportState audioTransportState;
    Analyser* analyser {nullptr};
    std::array<Filter*, NUMBER_OF_FILTERS> filter = {nullptr, nullptr, nullptr};
    
};
