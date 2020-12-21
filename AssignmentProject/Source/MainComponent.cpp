#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(Audio& a) :  audioVisualiser(2), audio(a)
{
    setSize (900, 750);
    //setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(audioFilePlayerComponent);
    audioFilePlayerComponent.setAudioFilePlayer(audio.getAudioFilePlayer());
    analyserComponent.setAnalyser(audio.getAnalyser());
    
    audio.setAudioVisualiserComponent(&audioVisualiser);
    addAndMakeVisible(audioVisualiser);
    audioVisualiser.setBufferSize(512);
    audioVisualiser.setSamplesPerBlock(256);
    audioVisualiser.setColours(Colours::whitesmoke, Colours::blue);
    
    addAndMakeVisible(analyserComponent);
    
    
    
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    int margin = 10;
    auto localBoundsWithMargin = getLocalBounds().reduced(margin);
    auto filePlayerArea = localBoundsWithMargin.removeFromTop(130);
    audioFilePlayerComponent.setBounds(filePlayerArea);
    
//    auto audioVisualiserArea = localBoundsWithMargin.removeFromTop(100);
//    audioVisualiser.setBounds(audioVisualiserArea);
    
    auto analyserArea = localBoundsWithMargin.removeFromTop(400);
    analyserComponent.setBounds(analyserArea.reduced(UIElementProperties::buttonPadding));
    
    
}
