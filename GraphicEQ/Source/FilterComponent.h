/*
  ==============================================================================

    FilterComponent.h
    Created: 22 Dec 2020 1:15:04pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Filter.h"
#include "UIElementProperties.h"
#include <memory>
#include "FilterResponseCurveComponent.h"
#include "AverageFilterResponseCurveComponent.h"
#include "Filter.h"

/** A component class for controlling a Filter object through UI elements.*/
/** Allows for the control of the filter frequency, resonance and gain through rotary sliders. Can also pass filter magnitude data to a FilterResponseCurveComponent.
 @param vt  a reference to a ValueTree object where the filter parameters will be stored.
 @param type    a filterType enum specifying the type of IIR filter (low shelf, high shelf or peak).
 @see filterType
 @see Filter
 @see FilterResponseCurveComponent*/
class FilterComponent : public Component,
                        public Button::Listener,
                        public ValueTree::Listener
{
public:
    /** Constructor
     @param vt      specifies the value tree to attach to UI elements
     @param type    speifies the type of the filter*/
    FilterComponent(ValueTree& vt, enum filterType type);
    
    /** Destructor */
    ~FilterComponent();
    
    //Component
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    /** Sets the filter that this component controls
     @param filterRef   a pointer to a Filter object
     @see Filter*/
    void setFilter(Filter* filterRef);
    
    /** Sets the filter response curve component that this component controls
     @param frcc   a pointer to a FilterResponseCurveComponent object
     @see FilterResponseCurveComponent*/
    void setFilterResponseComponent(FilterResponseCurveComponent* frcc);
    
    /** Sets the AverageFilterResponseCurveComponent that this component sends data to
     @param afrcc   a pointer to an AverageFilterResponseCurveComponent object
     @see AverageFilterResponseCurveComponent*/
    void setAverageFilterResponseComponent(AverageFilterResponseCurveComponent* afrcc);
    
    /** Passes a new parameter value to the filter that this component controls
     @param parameterName   specifies the name of the parameter (frequency/resonance/gain)
     @param parameterValue  specifies the value of the parameter*/
    void updateFilterParameter(String parameterName, float parameterValue);
    
    //Button listener
    void buttonClicked(Button* button) override;
    
    //ValueTree listener
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    
    /** Updates all response curves affected by the filter that this component controls*/
    void updateResponseCurves();
    
private:
    Slider frequencySlider;
    Slider gainSlider;
    Slider resonanceSlider;
    TextButton filterOn;
    
    std::unique_ptr<ValueTreeSliderAttachment> frequencySliderAttachment;
    std::unique_ptr<ValueTreeSliderAttachment> resonanceSliderAttachment;
    std::unique_ptr<ValueTreeSliderAttachment> gainSliderAttachment;
    
    FilterResponseCurveComponent* filterResponseComponent {nullptr};
    Filter* filter {nullptr};
    
    ValueTree filterSubTree;
    ValueTree& valueTree;
    
    float filterType = 0;;
    
    AverageFilterResponseCurveComponent* averageFilterResponseCurveComponent {nullptr};
    
};
