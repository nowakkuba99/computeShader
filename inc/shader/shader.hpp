#ifndef SHADER_H
#define SHADER_H

/* Include openGL libraries */
#include <glad/glad.h>

/* Include other libraries */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

/* Shader Class */
class Shader
{
    /* Variables */
    unsigned int shaderProgramID;    //Program ID
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
public:
    /* Constructors */
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);   //Read from file, compile and link
    /* Destructor */
    ~Shader()
    {
        glDeleteProgram(shaderProgramID);
    }
    /* Functions */
    void makeActive(); //Make current shader program active
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void renderQuad();
};


#endif /* SHADER_H */
