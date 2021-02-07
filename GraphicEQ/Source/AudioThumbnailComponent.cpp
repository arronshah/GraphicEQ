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
        audioThumbnailCache(2),
        thumbnailImage(Image::RGB, 880, 80, true)
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
    audioThumbnailArea = getLocalBounds().removeFromBottom(80).reduced(UIElementProperties::buttonPadding);
    
    if(audioThumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g);
    else
        paintIfFileLoaded(g);
}

void AudioThumbnailComponent::paintIfNoFileLoaded (juce::Graphics& g)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (audioThumbnailArea);
    g.setColour (juce::Colours::white);
    g.drawFittedText ("No File Loaded", audioThumbnailArea, juce::Justification::centred, 1);
}

void AudioThumbnailComponent::paintIfFileLoaded (juce::Graphics& g)
{
    g.drawImage(thumbnailImage, 0, 0, thumbnailImage.getWidth(), thumbnailImage.getHeight(), 0, 0, getWidth(), getHeight());
    
    g.setColour(Colours::white);
    double currentAudioPosition = audioFilePlayer->getCurrentPosition();
    float drawPosition = (currentAudioPosition / audioFileLength) * (float) audioThumbnailArea.getWidth() + (float) audioThumbnailArea.getX();
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
    {
        cacheAudioThumbnailImage();
        repaint();
    }
}

void AudioThumbnailComponent::cacheAudioThumbnailImage()
{
    Graphics g2(thumbnailImage);
    g2.setColour (juce::Colours::darkgrey);
    g2.fillRect (audioThumbnailArea);
    g2.setColour (juce::Colours::darkturquoise);
    audioFileLength = audioThumbnail.getTotalLength();
    audioThumbnail.drawChannels (g2, audioThumbnailArea, 0.0, audioFileLength, 1.0f);
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
