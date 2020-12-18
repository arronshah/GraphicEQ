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
        audioFilePlayer(nullptr),
        audioThumbnailCache(5),
        audioThumbnail(512, formatManager, audioThumbnailCache)
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
    
    audioThumbnail.addChangeListener(this);
    startTimer(40);
    
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
            audioFilePlayer->loadFile(audioFile, audioThumbnail);
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

void AudioFilePlayerComponent::paint (juce::Graphics& g)
{
    auto audioThumbnailArea = getLocalBounds().removeFromTop(130).removeFromBottom(80);
    
    if(audioThumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, audioThumbnailArea.reduced(UIElementProperties::buttonPadding));
    else
        paintIfFileLoaded(g, audioThumbnailArea.reduced(UIElementProperties::buttonPadding));
}

void AudioFilePlayerComponent::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailArea)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (audioThumbnailArea);
    g.setColour (juce::Colours::white);
    g.drawFittedText ("No File Loaded", audioThumbnailArea, juce::Justification::centred, 1);
}

void AudioFilePlayerComponent::paintIfFileLoaded (juce::Graphics& g, juce::Rectangle<int> audioThumbnailArea)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (audioThumbnailArea);
    
    g.setColour (juce::Colours::darkturquoise);
    
    float audioFileLength = audioThumbnail.getTotalLength();
    audioThumbnail.drawChannels (g, audioThumbnailArea, 0.0, audioFileLength, 1.0f);
    
    g.setColour(Colours::white);
    double currentAudioPosition = audioFilePlayer->getCurrentPosition();
    auto drawPosition = (currentAudioPosition / audioFileLength) * (float) audioThumbnailArea.getWidth() + (float) audioThumbnailArea.getX();
    g.drawLine (drawPosition, (float) audioThumbnailArea.getY(), drawPosition,
                (float) audioThumbnailArea.getBottom(), 2.0f);
    
    auto x = audioThumbnailArea.removeFromLeft(drawPosition - UIElementProperties::buttonPadding);
    g.setColour(Colours::grey);
    g.setOpacity(0.7);
    g.fillRect(x);
    
}

void AudioFilePlayerComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &audioThumbnail)
    {
        repaint();
    }
}

void AudioFilePlayerComponent::timerCallback()
{
    repaint();
}
