#pragma once

#include <JuceHeader.h>
#include "AudioFilePlayerComponent.hpp"
#include "Audio.h"
#include "AnalyserComponent.h"
#include "FilterComponent.h"
#include "FilterResponseCurveComponent.h"
#include "LookAndFeel.h"
#include "LevelMeterComponent.h"
#include "OpenGLComponent.h"

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
    Audio& audio;
    AnalyserComponent analyserComponent;
    ValueTree valueTree;
    LevelMeterComponent levelMeter;
    
    std::array<FilterComponent, 3> filterComponent =
    {{
        {valueTree, filterType::LowPass},
        {valueTree, filterType::BandPass},
        {valueTree, filterType::HighPass}
    }};
    
    std::array<FilterResponseCurveComponent, 3> filterResponseCurveComponent;
    
    UILookAndFeel newLookAndFeel;
    
    OpenGLComponent openGLComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};