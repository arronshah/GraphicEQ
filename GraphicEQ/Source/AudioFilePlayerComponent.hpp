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
    
    /** Toggles the text of the Play/Pause button to reflect state*/
    void togglePlayPauseButtonText();
    
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
