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
    /** Constructor */
    AudioThumbnailComponent();
    
    /** Destructor */
    ~AudioThumbnailComponent();
    
    /** Paints a placeholder indicating no file is currently loaded
     @param g   specifies a graphics object to draw with */
    void paintIfNoFileLoaded (juce::Graphics& g);
    
    /** Paints the waveform of the loaded audio file
     @param g   specifies a graphics object to draw with*/
    void paintIfFileLoaded (juce::Graphics& g);
    
    /** Sets the AudioFilePlayer that this component controls
     @param afp     a pointer to an AudioFilePlayer
     @see AudioFilePlayer*/
    void setAudioFilePlayer(AudioFilePlayer* afp);
    
    /** Sets the source of the AudioThumbnail
     @param newFile     specifies the source file to be passed to the AudioThumnail*/
    void setSource(const File& newFile);
    
    //ChangeListener
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    /** Caches the audio waveform to an image for more efficient repainting*/
    void cacheAudioThumbnailImage();
    
    //Component
    void paint (juce::Graphics& g) override;
    void mouseDown (const MouseEvent& event) override;
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    
    
    
    //Timer
    void timerCallback() override;
 
private:
    AudioFormatManager formatManager;
    AudioThumbnail audioThumbnail;
    AudioThumbnailCache audioThumbnailCache;
    AudioFilePlayer* audioFilePlayer;
    bool mouseInsideComponent;
    
    Image thumbnailImage;
    float audioFileLength;
    Rectangle<int> audioThumbnailArea;
    float prevDrawPosition;
    
};
