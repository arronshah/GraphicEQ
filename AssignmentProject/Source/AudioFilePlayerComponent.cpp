//
//  AudioFilePlayerComponent.cpp
//  AssignmentProject - App
//
//  Created by Arron Shah on 17/12/2020.
//

#include "AudioFilePlayerComponent.hpp"

AudioFilePlayerComponent::AudioFilePlayerComponent() :
        playPauseButton(">"),
        stopButton("Stop"),
        audioFilePlayer(nullptr)
{
    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    
    formatManager.registerBasicFormats();
    loadFileButton = std::make_unique<FilenameComponent> ("audiofile",
                                                       File(),
                                                       true, false, false,
                                                       formatManager.getWildcardForAllFormats(),
                                                       String(),
                                                       "Select an audio file...");
    loadFileButton->addListener(this);
    addAndMakeVisible (loadFileButton.get());
    
    addAndMakeVisible(audioThumbnailComponent);
}

void AudioFilePlayerComponent::resized()
{
    auto bounds = getLocalBounds();
    auto row = bounds.removeFromTop(50);
    auto audioFilePlayerControlArea = row.removeFromLeft(row.getWidth()/7);
    
    auto playPauseButtonArea = audioFilePlayerControlArea.removeFromLeft(audioFilePlayerControlArea.getWidth()/2);
    
    playPauseButton.setBounds(playPauseButtonArea.reduced(UIElementProperties::buttonPadding));
    stopButton.setBounds(audioFilePlayerControlArea.reduced(UIElementProperties::buttonPadding));
    loadFileButton->setBounds(row.reduced(UIElementProperties::buttonPadding));
    
    audioThumbnailComponent.setBounds(bounds.removeFromBottom(80));
    
}

void AudioFilePlayerComponent::paint (juce::Graphics& g)
{
    
    
}

void AudioFilePlayerComponent::buttonClicked(Button* button)
{
    if (button == &playPauseButton)
    {
        if(audioFilePlayer->isPlaying())
            audioFilePlayer->changeState(Pause);
        else
            audioFilePlayer->changeState(Play);
        
        togglePlayPauseButtonText();
    }
    else if (button == &stopButton)
    {
        if (audioFilePlayer->isPlaying())
        {
            togglePlayPauseButtonText();
        }
        audioFilePlayer->changeState(Stop);
    }
}

void AudioFilePlayerComponent::togglePlayPauseButtonText()
{
    String newText = (playPauseButton.getButtonText() == ">") ? "||" : ">";
    playPauseButton.setButtonText(newText);
}

void AudioFilePlayerComponent::filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == loadFileButton.get())
    {
        File audioFile (loadFileButton->getCurrentFile().getFullPathName());
        
        if(audioFilePlayer != nullptr && audioFile.existsAsFile())
        {
            audioFilePlayer->loadFile(audioFile);
            audioThumbnailComponent.setSource(audioFile);
        }
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "sdaTransport",
                                         "Couldn't open file!\n\n");
        }
    }
}

void AudioFilePlayerComponent::setAudioFilePlayer(AudioFilePlayer *afp)
{
    audioFilePlayer = afp;
    audioThumbnailComponent.setAudioFilePlayer(afp);
}


