/*
  ==============================================================================

    FilterResponseCurveComponent.h
    Created: 22 Dec 2020 6:14:38pm
    Author:  Arron Shah

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Filter.h"

class FilterResponseCurveComponent : public Component
{
public:
    FilterResponseCurveComponent();
    ~FilterResponseCurveComponent() = default;
    void paint(Graphics& g);
    void resized();
    void drawResponseCurve();
    void setFilter(Filter* filterRef);
    void updatePath();
private:
    Path responseCurve;
    Filter* filter {nullptr};
};
