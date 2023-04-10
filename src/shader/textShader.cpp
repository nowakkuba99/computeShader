/* Include class header file */
#include "../../inc/shader/textShader.hpp"


/*
    Function used to load vertex and fragment shaders code from files,
    compile them and link into shader Program.
    Arguments: vertex and fragment shader paths
*/
TextShader::TextShader(const char* vertexShaderPath, const char* fragmentShaderPath)
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
    catch (const std::exception& e)
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
    glShaderSource(vertex, 1, &vertexShaderCodeChar, NULL);    //Link shader code
    glCompileShader(vertex);                                //Compile shader
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compilationStatus);     //Get comilation status
    if (!compilationStatus)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    /* Fragment shader */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);              //Create shader
    glShaderSource(fragment, 1, &fragmentShaderCodeChar, NULL);    //Link shader code
    glCompileShader(fragment);                                //Compile shader
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compilationStatus);     //Get comilation status
    if (!compilationStatus)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    /* Shader program creation */
    shaderProgramID = glCreateProgram();        //Create program
    glAttachShader(shaderProgramID, vertex);     //Attach vertex shader
    glAttachShader(shaderProgramID, fragment);   //Attach fragment shader
    glLinkProgram(shaderProgramID);             //Link shader program

    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &compilationStatus);
    if (!compilationStatus)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }

    /* Delete shader objects */
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    /* Init textRenderer */
    //textRender.initFreetype("C:\\Users\\nowak\\source\\repos\\ComputeShader_LISA\\fonts\\consolab.ttf");
    textRender.initFreetype("fonts\\consolab.ttf");
}

/*
    Function to activate shader program
*/
void TextShader::makeActive()
{
    glUseProgram(shaderProgramID);
}

/*
    Function used to set uniform values
*/
void TextShader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}
void TextShader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void TextShader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void TextShader::initBuffers()
{
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void TextShader::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    makeActive();
    glUniform3f(glGetUniformLocation(shaderProgramID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        TextRenderer::Character ch = textRender.Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}