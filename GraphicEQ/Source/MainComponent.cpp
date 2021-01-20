#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(Audio& a) : audio(a), valueTree("PARAMETERS")
{
    setSize (900, 750);
    setLookAndFeel(&newLookAndFeel);
    addAndMakeVisible(audioFilePlayerComponent);
    audioFilePlayerComponent.setAudioFilePlayer(audio.getAudioFilePlayer());
    
    analyserComponent.setAnalyser(audio.getAnalyser());
    
    addAndMakeVisible(analyserComponent);
    
    for(int i = 0; i < filterComponent.size(); i++)
    {
        addAndMakeVisible(filterComponent[i]);
        filterComponent[i].setFilter(audio.getFilter(i));
        filterComponent[i].createValueTreeAttachments(i);
        
        addAndMakeVisible(filterResponseCurveComponent[i]);
        filterComponent[i].setFilterResponseComponent(&filterResponseCurveComponent[i]);
    }
    
    addAndMakeVisible(levelMeter);
    levelMeter.setAnalyser(audio.getAnalyser());
    levelMeter.startTimer(30);
    
    //addAndMakeVisible(openGLComponent);
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
    auto levelMeterArea = analyserArea.removeFromRight(35);
    
    analyserComponent.setBounds(analyserArea.reduced(UIElementProperties::buttonPadding));
    levelMeter.setBounds(levelMeterArea.reduced(UIElementProperties::buttonPadding));
    
    for (int i = 0; i < 3; i++)
        filterResponseCurveComponent[i].setBounds(analyserArea.reduced(UIElementProperties::buttonPadding));
    
    auto filterControlArea = localBoundsWithMargin.removeFromTop(220);
    auto bandControlWidth = filterControlArea.getWidth() / 6;
    
    for(auto& filter : filterComponent)
        filter.setBounds(filterControlArea.removeFromLeft(bandControlWidth));
    
    openGLComponent.setBounds(analyserArea.reduced(UIElementProperties::buttonPadding));
    
    
}
