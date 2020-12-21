/*
  ==============================================================================

    AnalyserComponent.cpp
    Created: 21 Dec 2020 11:14:09am
    Author:  Arron Shah

  ==============================================================================
*/

#include "AnalyserComponent.h"

AnalyserComponent::AnalyserComponent()
{
    startTimer(30);
    
}

void AnalyserComponent::paint(Graphics& g)
{
    if(!gridIsDrawn)
        drawGrid(g);
}

void AnalyserComponent::drawGrid(Graphics& g)
{
    const auto bounds = getLocalBounds();
    const auto height = bounds.getHeight();
    const auto width = bounds.getWidth();
    
    g.setColour(Colours::lightgrey);
    g.setOpacity(0.3);
    for(auto& frequency : gridFrequencies)
    {
        float linePosition = roundToInt(frequencyToProportion(frequency) * width);
        g.drawVerticalLine(linePosition, 0, height);
    }
}

float AnalyserComponent::frequencyToProportion (float freq)
{
    return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
}

void AnalyserComponent::paintOverChildren(Graphics& g)
{
    if(analyser != nullptr)
        drawPath(g);
}

void AnalyserComponent::drawPath(Graphics& g)
{
    
    path.clear();
    g.setColour (juce::Colours::paleturquoise);
    
    auto width  = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();
    
    float startPointX = (float) juce::jmap (0, 0, windowSize - 1, 0, width);
    float startPointY = juce::jmap (windowData[0], 0.0f, 1.0f, (float) height, 0.0f);
    
    if(!isnan(startPointX) || !isnan(startPointY))
        path.startNewSubPath(startPointX, startPointY);
    
    for (int i = 1; i < windowSize; i++)
    {
        float pointX = (float) juce::jmap (i, 0, windowSize - 1, 0, width);
        float pointY = juce::jmap (windowData[i], 0.0f, 1.0f, (float) height, 0.0f);
        path.lineTo( pointX, pointY);
    }
    
    Point<float> end((float) width, (float) height);
    Point<float> start(0.f, (float) height);
    path.lineTo(end);
    path.lineTo(start);
    path.closeSubPath();
    g.strokePath(path, PathStrokeType(1.f));
    ColourGradient gradient(Colours::darkturquoise, start, Colours::mediumpurple, end, true);
    FillType fill(gradient);
    fill.setOpacity(0.4);
    g.setFillType(fill);
    g.fillPath(path);
}

void AnalyserComponent::timerCallback()
{
    if (analyser != nullptr)
    {
        if (analyser->nextFftBlockIsReady())
        {
            drawNextFrame();
            analyser->setNextFftBlockIsReady(false);
            repaint();
        }
    }
}

void AnalyserComponent::resized()
{
    
}

void AnalyserComponent::drawNextFrame()
{
    if (analyser != nullptr)
    {
        fftData = analyser->getFftData();
        
        auto mindB = -100.0f;
        auto maxdB =    0.0f;
        
        for (int i = 0; i < windowSize; ++i)                         // [3]
        {
            auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) windowSize) * 0.2f);
            auto fftDataIndex = juce::jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (fftData[fftDataIndex])
                                                   - juce::Decibels::gainToDecibels ((float) fftSize)),
                                     mindB, maxdB, 0.0f, 1.0f);
            
            windowData[i] = level;                                   // [4]\
            
        }
    }
}

void AnalyserComponent::setAnalyser(Analyser* analyserRef)
{
    analyser = analyserRef;
}
