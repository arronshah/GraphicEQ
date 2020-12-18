/*
  ==============================================================================

    AudioThumbnailComponent.h
    Created: 18 Dec 2020 2:09:37pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "UIElementProperties.h"
#include "AudioFilePlayer.h"

class AudioThumbnailComponent : public Component,
                                private ChangeListener,
                                private Timer
{
public:
    AudioThumbnailComponent();
    ~AudioThumbnailComponent();
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void paint (juce::Graphics& g) override;
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded (juce::Graphics& g, juce::Rectangle<int> thumbnailBounds);
    void timerCallback() override;
    void mouseDown (const MouseEvent& event) override;
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    void setAudioFilePlayer(AudioFilePlayer* afp);
    void setSource(const File& newFile);
private:
    AudioFormatManager formatManager;
    AudioThumbnail audioThumbnail;
    AudioThumbnailCache audioThumbnailCache;
    AudioFilePlayer* audioFilePlayer;
    bool mouseInsideComponent;
    
};
