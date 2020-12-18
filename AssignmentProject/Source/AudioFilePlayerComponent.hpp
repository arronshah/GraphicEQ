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
    AudioFilePlayerComponent();
    ~AudioFilePlayerComponent() = default;
    void resized() override;
    void buttonClicked(Button* button) override;
    void togglePlayPauseButtonText();
    void paint (juce::Graphics&) override;
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
