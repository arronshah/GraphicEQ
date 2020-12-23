#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(Audio& a) : audio(a)
{
    setSize (900, 750);
    //setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(audioFilePlayerComponent);
    audioFilePlayerComponent.setAudioFilePlayer(audio.getAudioFilePlayer());
    
    analyserComponent.setAnalyser(audio.getAnalyser());
    
    addAndMakeVisible(analyserComponent);
    
    for(int i = 0; i < filterComponent.size(); i++)
    {
        addAndMakeVisible(filterComponent[i]);
        filterComponent[i].setFilter(audio.getFilter(i), i);
        
        addAndMakeVisible(filterResponseCurveComponent[i]);
        filterComponent[i].setFilterResponseComponent(&filterResponseCurveComponent[i], i);
    }
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
    
    auto analyserArea = localBoundsWithMargin.removeFromTop(400);
    analyserComponent.setBounds(analyserArea.reduced(UIElementProperties::buttonPadding));
    
    for (int i = 0; i < 3; i++) {
        filterResponseCurveComponent[i].setBounds(analyserArea.reduced(UIElementProperties::buttonPadding));
    }
    
    auto filterControlArea = localBoundsWithMargin.removeFromTop(170);
    auto bandControlWidth = filterControlArea.getWidth() / 5;
    
    for(auto& filter : filterComponent)
    {
        filter.setBounds(filterControlArea.removeFromLeft(bandControlWidth));
    }
    
}
