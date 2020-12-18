#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(Audio& a) :  audioVisualiser(2), audio(a)
{
    setSize (1000, 600);
    addAndMakeVisible(audioFilePlayerComponent);
    audioFilePlayerComponent.setFilePlayer(audio.getAudioFilePlayer());
    
//    addAndMakeVisible(audioVisualiser);
//    audioVisualiser.setBufferSize(512);
//    audioVisualiser.setSamplesPerBlock(256);
//    audioVisualiser.setColours(Colours::whitesmoke, Colours::blue);
    
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
    auto area = getLocalBounds().reduced(margin);
    audioFilePlayerComponent.setBounds(area);
    
//    auto audioVisualiserArea = area.removeFromTop(200).removeFromBottom(100);
//    audioVisualiser.setBounds(audioVisualiserArea);
}
