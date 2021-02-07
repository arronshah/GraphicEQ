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
    if(!gridIsCached)
        cacheGrid();
    
    g.drawImage(*grid, 0, 0, grid->getWidth(), grid->getHeight(), 0, 0, getWidth(), getHeight());
}

void AnalyserComponent::cacheGrid()
{
    grid = new Image(Image::RGB, getWidth(), getHeight(), true);
    
    Graphics g(*grid);
    
    const auto bounds = getLocalBounds();
    const auto height = bounds.getHeight() - 20;
    const auto width = bounds.getWidth();
    
    g.setColour(Colours::lightgrey);
    g.setOpacity(0.1);
    
    float linePosition = 0;
    
    for(auto& frequency : gridFrequencyValues)
    {
        linePosition = roundToInt(frequencyToDecimal(frequency) * width);
        g.drawVerticalLine(linePosition, 0, height);
    }
    
    for(auto& gainLabel : gridGainValues)
    {
        g.setOpacity(0.5);
        int linePosition = roundToInt(jmap (gainLabel, -84.f, 0.f, 1.0f, 0.0f) * height);
        String gainLabelAsString;
        gainLabelAsString << gainLabel;
        g.drawText(gainLabelAsString, width - 45, linePosition, 40, 40, Justification::topRight);
        
        g.setOpacity(0.1);
        g.drawHorizontalLine(linePosition, 0, width);
    }
    
    g.setOpacity(0.5);
    for(auto& frequencyLabel : gridFrequencyLabels)
    {
        linePosition = roundToInt(frequencyToDecimal(frequencyLabel) * width);
        String frequencyAsString;
        
        if(frequencyLabel >= 1000)
        {
            frequencyAsString << (frequencyLabel / 1000) << "k";
        }
        else
        {
            frequencyAsString << frequencyLabel;
        }
        
        g.drawText(frequencyAsString, linePosition, height, 40, 20, Justification::left);
    }
    
    gridIsCached = true;
}

float AnalyserComponent::frequencyToDecimal (float freq)
{
    return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
}

void AnalyserComponent::paintOverChildren(Graphics& g)
{
//    if(analyser != nullptr)
//        drawPath(g);
}

void AnalyserComponent::drawPath(Graphics& g)
{
    path.clear();
    g.setColour (juce::Colours::paleturquoise);

    auto width  = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight() - 20;

    path.startNewSubPath(0.f, height);
    std::array<Point<float>, 3> points;

    for (int i = 1; i < windowSize; i+=2)
    {
        for(int x = 0; x < 3; x++)
            points[x] = getScaledPoint(i+x);

        float cX = 2 * points[1].getX() - 0.5 * (points[0].getX() + points[2].getX());
        float cY = 2 * points[1].getY() - 0.5 * (points[0].getY() + points[2].getY());
        Point<float> controlPoint(cX, cY);

        if(!isnan(cY)) //remove the need for this condition by fixing repainting issues
        {
            path.lineTo(points[0]);
            path.quadraticTo(controlPoint, points[2]);
        }
    }

    Point<float> end((float) width, (float) height);
    Point<float> start(0.f, (float) height);
    path.lineTo(end);
    path.lineTo(start);
    path.closeSubPath();
    g.strokePath(path, PathStrokeType(1.5f, PathStrokeType::curved, PathStrokeType::rounded));
    ColourGradient gradient(Colours::darkturquoise, start, Colours::mediumpurple, end, true);
    FillType fill(gradient);
    fill.setOpacity(0.4);
    g.setFillType(fill);
    g.fillPath(path);
}

void AnalyserComponent::pushPointsToOpenGLContext()
{
    std::array<Point<float>, windowSize> pointsArr;
    
    for (int i = 1; i < windowSize; i++)
        pointsArr[i] = getScaledPoint(i);
    
    if(openGLComponent != nullptr)
        openGLComponent->updatePoints(pointsArr);
}

void AnalyserComponent::setOpenGLComponent(OpenGLComponent* glRef)
{
    openGLComponent = glRef;
}

Point<float> AnalyserComponent::getScaledPoint(int windowDataIndex)
{
    float x = juce::jmap (windowDataIndex, 0, windowSize - 1, 0, getWidth());
    float y = juce::jmap (windowData[windowDataIndex], 0.0f, 1.0f, (float) getHeight() - 20, 0.0f);
    
    return Point<float>(x, y);
}

void AnalyserComponent::timerCallback()
{
    if (analyser != nullptr)
    {
        if (analyser->nextFftBlockIsReady())
        {
            prepareNextFrame();
            analyser->setNextFftBlockIsReady(false);
            pushPointsToOpenGLContext();
//            repaint();
        }
    }
}

void AnalyserComponent::resized()
{
    
}

void AnalyserComponent::prepareNextFrame()
{
    if (analyser != nullptr)
    {
        fftData = analyser->getFftData();
        
        auto mindB = -100.0f;
        auto maxdB =    0.0f;
        
        for (int i = 0; i < windowSize; ++i)
        {
            auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) windowSize) * 0.2f);
            auto fftDataIndex = jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = jmap (jlimit (mindB, maxdB, Decibels::gainToDecibels (fftData[fftDataIndex])
                                                   - Decibels::gainToDecibels ((float) fftSize)),
                                     mindB, maxdB, 0.0f, 1.0f);
            
            windowData[i] = level;
            
        }
    }
}

void AnalyserComponent::setAnalyser(Analyser* analyserRef)
{
    analyser = analyserRef;
}
