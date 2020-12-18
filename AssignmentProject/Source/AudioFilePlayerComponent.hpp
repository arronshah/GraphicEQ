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

class AudioFilePlayerComponent : public Component,
                                 private Button::Listener,
                                 private FilenameComponentListener,
                                 private ChangeListener,
                                 private Timer
{
public:
    AudioFilePlayerComponent();
    ~AudioFilePlayerComponent() = default;
    void resized() override;
    void buttonClicked(Button* button) override;
    void togglePlayPauseButtonText();
    void setFilePlayer(AudioFilePlayer* afp);
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void paint (juce::Graphics& g) override;
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded (juce::Graphics& g, juce::Rectangle<int> thumbnailBounds);
    void timerCallback() override;
    
private:
    void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) override;
    TextButton playPauseButton;
    TextButton stopButton;
    std::unique_ptr<FilenameComponent> loadFileButton;
    AudioFormatManager formatManager;
    AudioFilePlayer* audioFilePlayer;
    AudioThumbnailCache audioThumbnailCache;
    AudioThumbnail audioThumbnail;
    
};
