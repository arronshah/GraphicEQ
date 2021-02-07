/*
  ==============================================================================

    OpenGLComponent.cpp
    Created: 19 Jan 2021 4:28:05pm
    Author:  Arron Shah

  ==============================================================================
*/

#include "OpenGLComponent.h"

OpenGLComponent::OpenGLComponent()
{
    points.resize(128);
    // Setup a pixel format object to tell the context what level of
    // multisampling to use.
    OpenGLPixelFormat pixelFormat;
    pixelFormat.multisamplingLevel = 4; // Change this value to your needs.
    
    openGLContext.setPixelFormat(pixelFormat);
    
    openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
    openGLContext.setMultisamplingEnabled(true);
    setOpaque(true);
    openGLContext.setRenderer(this);
    openGLContext.setContinuousRepainting(true);
    openGLContext.setComponentPaintingEnabled(true);
    openGLContext.attachTo(*this);
    
}

OpenGLComponent::~OpenGLComponent()
{
    openGLContext.detach();
}

void OpenGLComponent::updatePoints(std::array<Point<float>, 128> newPoints)
{
    for(auto i = 0; i < points.size(); i++)
    {
        points[i].x = jmap(newPoints[i].getX(), 0.f, 835.f, -1.f, 1.f);
        points[i].y = jmap(newPoints[i].getY(), 370.f, 0.f, -1.f, 1.f);
    }
}

static void writeVec2(juce::MemoryOutputStream &out, const crushedpixel::Vec2 &vec) {
    // write vertices as two consecutive floats
    out.writeFloat(vec.x);
    out.writeFloat(vec.y);
}

void OpenGLComponent::paint(Graphics& g)
{
    //g.fillAll(Colours::red);
}

void OpenGLComponent::resized()
{
    
}

void OpenGLComponent::fillBuffer()
{
    // generate VAO
    openGLContext.extensions.glGenVertexArrays(1, &vaoHandle);
    openGLContext.extensions.glBindVertexArray(vaoHandle);
    
    // generate VBOs
    openGLContext.extensions.glGenBuffers(1, &vboHandle);
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    
    // generate polyline vertices
    auto vertices = crushedpixel::Polyline2D::create(
                                                     points, 0.005f,
                                                     crushedpixel::Polyline2D::JointStyle::ROUND,
                                                     crushedpixel::Polyline2D::EndCapStyle::ROUND);
    
    numPoints = (GLsizei) points.size();
    numVertices = (GLsizei) vertices.size();
    
    // write original points, followed by vertices, to buffer
    juce::MemoryOutputStream mos;
    for (auto &vertex : points) {
        writeVec2(mos, vertex);
    }
    for (auto &vertex : vertices) {
        writeVec2(mos, vertex);
    }
    
    openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, mos.getDataSize(), mos.getData(), GL_STATIC_DRAW);
}

void OpenGLComponent::newOpenGLContextCreated()
{
    // generate VAO
    openGLContext.extensions.glGenVertexArrays(1, &vaoHandle);
    openGLContext.extensions.glBindVertexArray(vaoHandle);
    
    // generate VBOs
    openGLContext.extensions.glGenBuffers(1, &vboHandle);
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    
    // generate polyline vertices
    auto vertices = crushedpixel::Polyline2D::create(
                                                     points, 0.0001f,
                                                     crushedpixel::Polyline2D::JointStyle::ROUND,
                                                     crushedpixel::Polyline2D::EndCapStyle::SQUARE);
    
    numPoints = (GLsizei) points.size();
    numVertices = (GLsizei) vertices.size();
    
    // write original points, followed by vertices, to buffer
    juce::MemoryOutputStream mos;
    for (auto &vertex : points) {
        writeVec2(mos, vertex);
    }
    for (auto &vertex : vertices) {
        writeVec2(mos, vertex);
    }
    
    openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, mos.getDataSize(), mos.getData(), GL_STATIC_DRAW);
    
    // generate Shader Program
    auto vert = openGLContext.extensions.glCreateShader(GL_VERTEX_SHADER);
    auto frag = openGLContext.extensions.glCreateShader(GL_FRAGMENT_SHADER);
    
    openGLContext.extensions.glShaderSource(vert, 1, &BinaryData::Passthrough_vert, &BinaryData::Passthrough_vertSize);
    openGLContext.extensions.glShaderSource(frag, 1, &BinaryData::PlainColor_frag, &BinaryData::PlainColor_fragSize);
    
    openGLContext.extensions.glCompileShader(vert);
    openGLContext.extensions.glCompileShader(frag);
    // no error checking, I don't make mistakes (lol)
    
    programHandle = openGLContext.extensions.glCreateProgram();
    openGLContext.extensions.glAttachShader(programHandle, vert);
    openGLContext.extensions.glAttachShader(programHandle, frag);
    openGLContext.extensions.glLinkProgram(programHandle);
    
    openGLContext.extensions.glDeleteShader(vert);
    openGLContext.extensions.glDeleteShader(frag);
    
    // get shader attribute locations
    posInAttribLocation = openGLContext.extensions.glGetAttribLocation(programHandle, "posIn");
    colorUniformLocation = openGLContext.extensions.glGetUniformLocation(programHandle, "color");
    
    // tell OpenGL to read the input data as pairs of floats
    openGLContext.extensions.glVertexAttribPointer(posInAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    openGLContext.extensions.glEnableVertexAttribArray(posInAttribLocation);
}

bool OpenGLComponent::checkShaderCompileStatus(GLuint shaderID) {
    GLint status;
    openGLContext.extensions.glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        openGLContext.extensions.glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        GLchar* log = new char[length + 1];
        openGLContext.extensions.glGetShaderInfoLog(shaderID, length, &length, &log[0]);
        std::cout<<"Error by shader compilation:"<<log;
        return false;
    }
    return true;
}

void OpenGLComponent::renderOpenGL()
{
    newOpenGLContextCreated();
    
    OpenGLHelpers::clear(Colours::black);
    
    openGLContext.extensions.glBindVertexArray(vaoHandle);
    openGLContext.extensions.glUseProgram(programHandle);
    
    {   // draw wireframe
        auto color = juce::Colours::lightgreen;
        openGLContext.extensions.glUniform4f(colorUniformLocation,
                        color.getFloatRed(),
                        color.getFloatGreen(),
                        color.getFloatBlue(),
                        color.getFloatAlpha());
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, numPoints, numVertices);
    }
    
//    {   // fill solid
//        auto color = juce::Colours::green.withAlpha(0.5f);
//        openGLContext.extensions.glUniform4f(colorUniformLocation,
//                        color.getFloatRed(),
//                        color.getFloatGreen(),
//                        color.getFloatBlue(),
//                        color.getFloatAlpha());
//
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        glDrawArrays(GL_TRIANGLES, numPoints, numVertices);
//    }
    
//    {   // draw original points
//        glPointSize(5);
//
//        auto color = juce::Colours::red;
//        openGLContext.extensions.glUniform4f(colorUniformLocation,
//                        color.getFloatRed(),
//                        color.getFloatGreen(),
//                        color.getFloatBlue(),
//                        color.getFloatAlpha());
//
//        glDrawArrays(GL_POINTS, 0, numPoints);
//    }
    
    openGLContext.extensions.glDeleteVertexArrays(1, &vaoHandle);
    openGLContext.extensions.glDeleteBuffers(1, &vboHandle);
    openGLContext.extensions.glDeleteProgram(programHandle);
    
    
}

void OpenGLComponent::openGLContextClosing()
{
    openGLContext.extensions.glDeleteVertexArrays(1, &vaoHandle);
    openGLContext.extensions.glDeleteBuffers(1, &vboHandle);
    openGLContext.extensions.glDeleteProgram(programHandle);
}
