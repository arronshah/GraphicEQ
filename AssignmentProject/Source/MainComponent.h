#pragma once

#include <JuceHeader.h>
#include "AudioFilePlayerComponent.hpp"
#include "Audio.h"
#include "AnalyserComponent.h"
#include "FilterComponent.h"
#include "FilterResponseCurveComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent(Audio& a);
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    //==============================================================================
    // Your private member variables go here...
    AudioFilePlayerComponent audioFilePlayerComponent;
    AudioVisualiserComponent audioVisualiser;
    Audio& audio;
    AnalyserComponent analyserComponent;
    FilterComponent filterComponent;
    FilterResponseCurveComponent filterResponseCurveComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
