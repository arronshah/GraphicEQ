//
//  AudioFilePlayerComponent.hpp
//  AssignmentProject - App
//
//  Created by Arron Shah on 17/12/2020.
//

#pragma once

#include <JuceHeader.h>
#include "UIElementProperties.h"
#include "AudioFilePlayer.h"
#include "AudioThumbnailComponent.h"

/** A component class for controlling the playback of an audio file loaded into an AudioFilePlayer object.*/
/** Contains buttons for play, stop and pause as well as a file browser.
 @see AudioFilePlayer*/
class AudioFilePlayerComponent : public Component,
                                 private Button::Listener,
                                 private FilenameComponentListener
{
public:
    /** Constructor */
    AudioFilePlayerComponent();
    
    /** Destructor */
    ~AudioFilePlayerComponent() = default;
    
    //Component
    void resized() override;
    void paint (juce::Graphics&) override;
    
    //Button listener
    void buttonClicked(Button* button) override;
    
    /** Sets the AudioFilePlayer that this component controls
     @param     a pointer to an AudioFilePlayer
     @see AudioFilePlayer*/
    void setAudioFilePlayer(AudioFilePlayer* afp);
    
private:
    void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) override;
    TextButton playPauseButton;
    TextButton stopButton;
    std::unique_ptr<FilenameComponent> loadFileButton;
    AudioFormatManager formatManager;
    AudioFilePlayer* audioFilePlayer;
    AudioThumbnailComponent audioThumbnailComponent;
};
