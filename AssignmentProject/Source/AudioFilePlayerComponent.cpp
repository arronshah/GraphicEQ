//
//  AudioFilePlayerComponent.cpp
//  AssignmentProject - App
//
//  Created by Arron Shah on 17/12/2020.
//

#include "AudioFilePlayerComponent.hpp"

AudioFilePlayerComponent::AudioFilePlayerComponent() :
        playButton("Play"),
        audioFilePlayer(nullptr)
{
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    loadFileButton = std::make_unique<FilenameComponent> ("audiofile",
                                                       File(),
                                                       true, false, false,
                                                       formatManager.getWildcardForAllFormats(),
                                                       String(),
                                                       "Select an audio file...");
    loadFileButton->addListener(this);
    addAndMakeVisible (loadFileButton.get());
    
}

void AudioFilePlayerComponent::resized()
{
    auto bounds = getLocalBounds();
    auto row = bounds.removeFromTop(100);
    auto col1 = row.removeFromLeft(row.getWidth()/7);
    playButton.setBounds(col1.reduced(UIElementProperties::buttonPadding));
    loadFileButton->setBounds(row.reduced(UIElementProperties::buttonPadding));
}

void AudioFilePlayerComponent::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        audioFilePlayer->setPlaying(!audioFilePlayer->isPlaying());
        togglePlayButtonText();
    }
}

void AudioFilePlayerComponent::togglePlayButtonText()
{
    String newText = (playButton.getButtonText() == "Play") ? "Stop" : "Play";
    playButton.setButtonText(newText);
}

void AudioFilePlayerComponent::filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == loadFileButton.get())
    {
        File audioFile (loadFileButton->getCurrentFile().getFullPathName());
        
        if(audioFilePlayer != nullptr && audioFile.existsAsFile())
        {
            audioFilePlayer->loadFile(audioFile);
        }
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "sdaTransport",
                                         "Couldn't open file!\n\n");
        }
    }
}

void AudioFilePlayerComponent::setFilePlayer(AudioFilePlayer *afp)
{
    audioFilePlayer = afp;
}
