/*
  ==============================================================================

    AudioThumbnailComponent.cpp
    Created: 18 Dec 2020 2:09:37pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "AudioThumbnailComponent.h"

AudioThumbnailComponent::AudioThumbnailComponent() :
        audioThumbnail(512, formatManager, audioThumbnailCache),
        audioThumbnailCache(2)
{
    formatManager.registerBasicFormats();
    audioThumbnail.addChangeListener(this);
    startTimer(40);
    
}

AudioThumbnailComponent::~AudioThumbnailComponent()
{
    audioThumbnail.setSource(nullptr); //unload file
}

void AudioThumbnailComponent::paint (juce::Graphics& g)
{
    Rectangle<int> audioThumbnailArea = getLocalBounds().removeFromBottom(80).reduced(UIElementProperties::buttonPadding);
    
    if(audioThumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, audioThumbnailArea);
    else
        paintIfFileLoaded(g, audioThumbnailArea);
}

void AudioThumbnailComponent::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailArea)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (audioThumbnailArea);
    g.setColour (juce::Colours::white);
    g.drawFittedText ("No File Loaded", audioThumbnailArea, juce::Justification::centred, 1);
}

void AudioThumbnailComponent::paintIfFileLoaded (juce::Graphics& g, juce::Rectangle<int> audioThumbnailArea)
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
    
    auto playedAudioArea = audioThumbnailArea.removeFromLeft(drawPosition - UIElementProperties::buttonPadding);
    g.setColour(Colours::grey);
    g.setOpacity(0.7);
    g.fillRect(playedAudioArea);
    
}

void AudioThumbnailComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &audioThumbnail)
        repaint();
}

void AudioThumbnailComponent::timerCallback()
{
    repaint();
}

void AudioThumbnailComponent::mouseDown (const MouseEvent& event)
{
    if(mouseInsideComponent)
    {
        int mouseXPosition = event.getMouseDownX() - 5;
        double newPosition = (mouseXPosition / float(getWidth())) * audioFilePlayer->getLengthOfFileInSeconds();
        audioFilePlayer->updatePosition(newPosition);
    }
}

void AudioThumbnailComponent::mouseEnter (const MouseEvent& event)
{
    mouseInsideComponent = true;
}

void AudioThumbnailComponent::mouseExit (const MouseEvent& event)
{
    mouseInsideComponent = false;
}

void AudioThumbnailComponent::setAudioFilePlayer(AudioFilePlayer *afp)
{
    audioFilePlayer = afp;
}

void AudioThumbnailComponent::setSource(const File& newFile)
{
    audioThumbnail.setSource(new FileInputSource(newFile));
}
