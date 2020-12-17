//
//  AudioFilePlayerComponent.cpp
//  AssignmentProject - App
//
//  Created by Arron Shah on 17/12/2020.
//

#include "AudioFilePlayerComponent.hpp"

AudioFilePlayerComponent::AudioFilePlayerComponent() :
        play("Play"),
        loadFile("Load")
{
    
    addAndMakeVisible(play);
    addAndMakeVisible(loadFile);
}

void AudioFilePlayerComponent::resized()
{
    auto bounds = getLocalBounds();
    auto row = bounds.removeFromTop(100);
    auto col1 = row.removeFromLeft(row.getWidth()/7);
    play.setBounds(col1.removeFromTop(col1.getHeight()/2).reduced(UIElementProperties::buttonPadding));
    loadFile.setBounds(col1.reduced(UIElementProperties::buttonPadding));
}
