/*
  ==============================================================================

    OpenGLComponent.h
    Created: 19 Jan 2021 4:28:05pm
    Author:  Arron Shah

  ==============================================================================
*/

#include <JuceHeader.h>

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
    
private:
    OpenGLContext openGLContext;
    
    struct Vertex
    {
        float position[2];
        float colour[4];
    };
    
    std::vector<Vertex> vertexBuffer;
    std::vector<GLuint> indexBuffer;
    
    GLuint vertexBufferObject;
    GLuint indexBufferObject;
    
    const char* vertexShader;
    String fragmentShader;
    GLuint vShader;
    GLuint fShader;
    
    std::unique_ptr<OpenGLShaderProgram> shaderProgram;
};
