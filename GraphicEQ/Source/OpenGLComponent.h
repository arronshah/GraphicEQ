/*
  ==============================================================================

    OpenGLComponent.h
    Created: 19 Jan 2021 4:28:05pm
    Author:  Arron Shah

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Polyline2D.h"

#pragma once

class OpenGLComponent : public Component,
                        public OpenGLRenderer
{
public:
    OpenGLComponent();
    ~OpenGLComponent();
    void paint(Graphics& g) override;
    void resized() override;
    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;
    bool checkShaderCompileStatus(GLuint shaderID);
    void updatePoints(std::array<Point<float>, 128> newPoints);
    void fillBuffer();
    
private:
    OpenGLContext openGLContext;
    
    std::vector<crushedpixel::Vec2> points;
    
    GLuint vaoHandle, vboHandle, programHandle;
    
    GLint posInAttribLocation;
    GLint colorUniformLocation;
    
    GLsizei numPoints, numVertices;
};
