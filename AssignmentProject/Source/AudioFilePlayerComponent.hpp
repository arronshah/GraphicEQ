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
                                 private FilenameComponentListener
{
public:
    AudioFilePlayerComponent();
    ~AudioFilePlayerComponent() = default;
    void resized() override;
    void buttonClicked(Button* button) override;
    void togglePlayButtonText();
    void setFilePlayer(AudioFilePlayer* afp);
    
private:
    void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) override;
    TextButton playButton;
    std::unique_ptr<FilenameComponent> loadFileButton;
    AudioFilePlayer* audioFilePlayer;
    
};
