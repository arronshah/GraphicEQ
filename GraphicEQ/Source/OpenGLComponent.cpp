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
    setOpaque(true);
    openGLContext.setRenderer(this);
    openGLContext.setContinuousRepainting(true);
    openGLContext.attachTo(*this);
}

OpenGLComponent::~OpenGLComponent()
{
    openGLContext.detach();
}

void OpenGLComponent::paint(Graphics& g)
{
    //g.fillAll(Colours::black);
}

void OpenGLComponent::resized()
{
    
}

void OpenGLComponent::newOpenGLContextCreated()
{
    //Generate a buffer and store its ID in vertexBufferObject variable
    openGLContext.extensions.glGenBuffers(1, &vertexBufferObject);
    openGLContext.extensions.glGenBuffers(1, &indexBufferObject);
    
    // Create 4 vertices each with a different colour.
    vertexBuffer = {
        // Vertex 0
        {
            { -0.5f, 0.5f },        // (-0.5, 0.5)
            { 1.f, 0.f, 0.f, 1.f }  // Red
        },
        // Vertex 1
        {
            { 0.5f, 0.5f },         // (0.5, 0.5)
            { 1.f, 0.5f, 0.f, 1.f } // Orange
        },
        // Vertex 2
        {
            { 0.5f, -0.5f },        // (0.5, -0.5)
            { 1.f, 1.f, 0.f, 1.f }  // Yellow
        },
        // Vertex 3
        {
            { -0.5f, -0.5f },       // (-0.5, -0.5)
            { 1.f, 0.f, 1.f, 1.f }  // Purple
        }
    };
    
    // We need 6 indices, 1 for each corner of the two triangles.
    indexBuffer = {
        0, 1, 2,
        0, 2, 3
    };
    
    //Bind the vertexBufferObject
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    
    // Send the vertices data.
    openGLContext.extensions.glBufferData(
                                          GL_ARRAY_BUFFER,                        // The type of data we're sending.
                                          sizeof(Vertex) * vertexBuffer.size(),   // The size (in bytes) of the data.
                                          vertexBuffer.data(),                    // A pointer to the actual data.
                                          GL_STATIC_DRAW                          // How we want the buffer to be drawn.
                                          );
    
    // Bind the IBO.
    openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    
    // Send the indices data.
    openGLContext.extensions.glBufferData(
                                          GL_ELEMENT_ARRAY_BUFFER,
                                          sizeof(GLuint) * indexBuffer.size(),
                                          indexBuffer.data(),
                                          GL_STATIC_DRAW
                                          );
    
    vertexShader =
        R"(
                // Input attributes.
                varying vec4 position;
                varying vec4 sourceColour;
    
                // Output to fragment shader.
                varying vec4 fragColour;
    
                void main()
                {
                    // Set the position to the attribute we passed in.
                    gl_Position = position;
                    
                    // Set the frag colour to the attribute we passed in.
                    // This value will be interpolated for us before the fragment
                    // shader so the colours we defined ateach vertex will create a
                    // gradient across the shape.
                    fragColour = sourceColour;
                }
            )";
    
            fragmentShader =
            R"(
    
            // The value that was output by the vertex shader.
            // This MUST have the exact same name that we used in the vertex shader.
            varying vec4 fragColour;
    
            void main()
            {
                // Set the fragment's colour to the attribute passed in by the
                // vertex shader.
                gl_FragColor = fragColour;
            }
            )";
    
//    openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
//    vShader = openGLContext.extensions.glCreateShader(GL_VERTEX_SHADER);
//    openGLContext.extensions.glShaderSource(vShader, 1, &vertexShader, NULL);
//    openGLContext.extensions.glCompileShader(vShader);
//    if (!checkShaderCompileStatus(vShader)) {
//        DBG("SHADER DID NOT COMPILE");
//    }
    
    
    // Create an instance of OpenGLShaderProgram
    shaderProgram.reset(new OpenGLShaderProgram(openGLContext));
    
    // Compile and link the shader.
    // Each of these methods will return false if something goes wrong so we'll
    // wrap them in an if statement
    if (shaderProgram->addVertexShader(vertexShader)
        && shaderProgram->addFragmentShader(fragmentShader)
        && shaderProgram->link())
    {
        // No compilation errors - set the shader program to be active
        shaderProgram->use();
    }
    else
    {
        // Oops - something went wrong with our shaders!
        // Check the output window of your IDE to see what the error might be.
        jassertfalse;
    }
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
    OpenGLHelpers::clear(Colours::black);
    shaderProgram->use();
    
    GLsizei stride = sizeof(Vertex);
    
    //Enable the position attribute.
    openGLContext.extensions.glVertexAttribPointer(
                                                   0,              // The attribute's index (AKA location).
                                                   2,              // How many values this attribute contains.
                                                   GL_FLOAT,       // The attribute's type (float).
                                                   GL_FALSE,       // Tells OpenGL NOT to normalise the values.
                                                   stride, // How many bytes to move to find the attribute with
                                                   // the same index in the next vertex.
                                                   nullptr         // How many bytes to move from the start of this vertex
                                                   // to find this attribute (the default is 0 so we just
                                                   // pass nullptr here).
                                                   );
    //openGLContext.extensions.glEnableVertexAttribArray(0);

   //  Enable to colour attribute.
    openGLContext.extensions.glVertexAttribPointer(
                                                   1,                              // This attribute has an index of 1
                                                   4,                              // This time we have four values for the
                                                   // attribute (r, g, b, a)
                                                   GL_FLOAT,
                                                   GL_FALSE,
                                                   stride,
                                                   (GLvoid*)(sizeof(float) * 2)    // This attribute comes after the
                                                   // position attribute in the Vertex
                                                   // struct, so we need to skip over the
                                                   // size of the position array to find
                                                   // the start of this attribute.
                                                   );
    openGLContext.extensions.glEnableVertexAttribArray(1);
    
    //glDrawArrays(GL_TRIANGLES, 0, sizeof(indexBuffer));
    
    glDrawElements(
                   GL_TRIANGLES,       // Tell OpenGL to render triangles.
                   6, // How many indices we have.
                   GL_UNSIGNED_INT,    // What type our indices are.
                   nullptr             // We already gave OpenGL our indices so we don't
                   // need to pass that again here, so pass nullptr.
                   );
    
    openGLContext.extensions.glDisableVertexAttribArray(0);
    openGLContext.extensions.glDisableVertexAttribArray(1);
}

void OpenGLComponent::openGLContextClosing()
{
    
}
