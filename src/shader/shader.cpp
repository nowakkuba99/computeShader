/* Include class header file */
#include "../../inc/shader/shader.hpp"


/*
    Function used to load vertex and fragment shaders code from files,
    compile them and link into shader Program.
    Arguments: vertex and fragment shader paths
*/
Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    /* Read shader code from file */
    std::string vertexShaderCode;       //Variable to store code
    std::string fragmentShaderCode;     //Variable to store code

    std::ifstream vertexShaderFile;     //File read object
    std::ifstream fragmentShaderFile;   //File read object

    vertexShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);    //Make sure ifstream objects can throw exception
    fragmentShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);    //Make sure ifstream objects can throw exception

    try
    {
        /* Open files */
        vertexShaderFile.open(vertexShaderPath);
        fragmentShaderFile.open(fragmentShaderPath);
        /* Define varialbes to read from files */
        std::stringstream vertexShaderStream, fragmentShaderStream;
        /* Read buffer */
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        /* Close files */
        vertexShaderFile.close();
        fragmentShaderFile.close();
        /* Convert stream files to string */
        vertexShaderCode = vertexShaderStream.str();
        fragmentShaderCode = fragmentShaderStream.str();
    }
    catch(const std::exception& e)
    {
        /* If failed print error message */
        std::cout << "ERROR::SHADER::FILE_NOT_CORRECTLY_READ \n";
        std::cerr << e.what() << '\n';
    }
    /* Convert from string to to const char* (needed for compile openGl functions) */
    const char* vertexShaderCodeChar = vertexShaderCode.c_str();
    const char* fragmentShaderCodeChar = fragmentShaderCode.c_str();

    /* Compile shaders */
    unsigned int vertex, fragment;  //Variables to store shaders id's
    int compilationStatus; //Variable to hold comilation status
    char infoLog[512];  //Variable to hold error message if needed

    /* Vertex shader */
    vertex = glCreateShader(GL_VERTEX_SHADER);              //Create shader
    glShaderSource(vertex,1,&vertexShaderCodeChar,NULL);    //Link shader code
    glCompileShader(vertex);                                //Compile shader
    glGetShaderiv(vertex,GL_COMPILE_STATUS,&compilationStatus);     //Get comilation status
    if(!compilationStatus)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    /* Fragment shader */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);              //Create shader
    glShaderSource(fragment,1,&fragmentShaderCodeChar,NULL);    //Link shader code
    glCompileShader(fragment);                                //Compile shader
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&compilationStatus);     //Get comilation status
    if(!compilationStatus)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
    }
    
    /* Shader program creation */
    shaderProgramID = glCreateProgram();        //Create program
    glAttachShader(shaderProgramID,vertex);     //Attach vertex shader
    glAttachShader(shaderProgramID,fragment);   //Attach fragment shader
    glLinkProgram(shaderProgramID);             //Link shader program

    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &compilationStatus);
    if(!compilationStatus)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }

    /* Delete shader objects */
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

/*
    Function to activate shader program
*/
void Shader::makeActive()
{
    glUseProgram(shaderProgramID);
}

/*
    Function used to set uniform values
*/
void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value); 
} 

/* 
    Function to render quad to screen    
*/
void Shader::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}