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
    std::fill(oldLevel.begin(), oldLevel.end(), 0.f);
}

void AnalyserComponent::paint(Graphics& g)
{
    if(!gridIsCached)
        cacheGrid();
    
    g.drawImage(*grid, 0, 0, grid->getWidth(), grid->getHeight(), 0, 0, getWidth(), getHeight());
    
    if(analyser != nullptr)
        drawPath(g);

}

void AnalyserComponent::cacheGrid()
{
    grid = std::make_unique<Image>(Image::RGB, getWidth(), getHeight(), true);
    
    Graphics g(*grid);
    
    const auto bounds = getLocalBounds();
    const auto height = bounds.getHeight() - 20;
    const auto width = bounds.getWidth();
    
    g.setColour(Colours::lightgrey);
    g.setOpacity(0.1);
    
    float linePosition = 0;
    
    //Draw logarithmic lines for frequency values across width of component
    for(auto& frequency : gridFrequencyValues)
    {
        linePosition = roundToInt(Helpers::frequencyToDecimal(frequency) * width);
        g.drawVerticalLine(linePosition, 0, height);
    }
    
    //Draw linear lines for gain values across height of component
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
    
    //Draw frequency labels onto lines
    for(auto& frequencyLabel : gridFrequencyLabels)
    {
        linePosition = roundToInt(Helpers::frequencyToDecimal(frequencyLabel) * width);
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

void AnalyserComponent::drawPath(Graphics& g)
{
    path.clear();
    g.setOpacity(1.f);
    g.setColour (juce::Colours::lightgrey);

    auto width  = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight() - 20;

    path.startNewSubPath(0.f, height);
    
    path.lineTo(getScaledPoint(0)); //FIX Y POINT BEING NAN
    
    std::array<Point<float>, 3> points;
    
    for (int i = 1; i < windowSize; i+=2) //This loop interpolates between 2 points and pass center as control point to allow Path to calculate quadratic - results in smoother curve
    {
        for(int x = 0; x < 3; x++)
            points[x] = getScaledPoint(i+x);

        float cPointX = 2 * points[1].getX() - 0.5 * (points[0].getX() + points[2].getX());
        float cPointY = 2 * points[1].getY() - 0.5 * (points[0].getY() + points[2].getY());
        Point<float> controlPoint(cPointX, cPointY);

        path.quadraticTo(controlPoint, points[2]);
    }

    path.lineTo(width, height);
    path.closeSubPath();
    g.strokePath(path, PathStrokeType(1.5f));
    g.setColour(Colours::darkgrey);
    g.setOpacity(0.5);
    g.fillPath(path);

}

void AnalyserComponent::pushPointsToOpenGLContext()
{
    std::array<Point<float>, windowSize> pointsArr;
    
    for (int i = 1; i < windowSize; i++)
        pointsArr[i] = getScaledPoint(i);
    
    //openGLComponent.updatePoints(pointsArr);
}

Point<float> AnalyserComponent::getScaledPoint(int windowDataIndex)
{

    windowDataIndex = jlimit(0.f, (float) windowSize - 1, (float) windowDataIndex);
    float height = getHeight() - 20;
    
    float x = jmap (windowDataIndex, 0, windowSize - 1, 0, getWidth());
    float y = jmap (windowData[windowDataIndex], 0.0f, 1.0f, (float) height, 0.0f);
    
    return Point<float>(x, (!isnan(y)) ? y : height );
}

void AnalyserComponent::timerCallback()
{
    if (analyser != nullptr)
    {
        if (analyser->nextFftBlockIsReady())
        {
            prepareNextFrame();
            analyser->setNextFftBlockIsReady(false);
            //pushPointsToOpenGLContext();
            repaint();
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
            //Credit to https://docs.juce.com/master/tutorial_spectrum_analyser.html
            //Skew x-axis to use logarithmic scale, retrieve the correct array index and then scale the retrived value between 0 and 1
            auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) windowSize) * 0.2f);
            auto fftDataIndex = jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = jmap (jlimit (mindB, maxdB, Decibels::gainToDecibels (fftData[fftDataIndex])
                                                   - Decibels::gainToDecibels ((float) fftSize)),
                                     mindB, maxdB, 0.0f, 1.0f);
            
            //apply slight filtering depending on whether level is rising or falling to prevent the graph from flickering as much
            float coeff = (level > oldLevel[i]) ? 0.1 : 0.0001;
            windowData[i] = coeff * level + (1 - coeff) * oldLevel[i];
            oldLevel[i] = level;
            
        }
    }
}

void AnalyserComponent::setAnalyser(Analyser* analyserRef)
{
    analyser = analyserRef;
}
