#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

/* Include openGL libraries */
#include <glad/glad.h>
#include <glm/glm.hpp>

/* Include other libraries */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../textRenderer/textRenderer.hpp"

/* Shader Class */
class TextShader
{
    /* Variables */
    unsigned int textVAO = 0;
    unsigned int textVBO = 0;
    TextRenderer textRender;
public:
    unsigned int shaderProgramID;    //Program ID
    /* Constructors */
    TextShader(const char* vertexShaderPath, const char* fragmentShaderPath);   //Read from file, compile and link
    /* Destructor */
    ~TextShader()
    {
        glDeleteProgram(shaderProgramID);
    }
    /* Functions */
    void makeActive(); //Make current shader program active
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void initBuffers();
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
};


#endif /* TEXT_SHADER_H */
