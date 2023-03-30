#include "../../inc/computeShader/computeShader.hpp"

/* --- Constructor --- */
ComputeShader::ComputeShader(const char* path, glm::uvec2 size)
{
    work_size = size;

    // read in shader code
    std::string compute_code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream file_stream;
        file_stream << file.rdbuf();
        file.close();

        compute_code = file_stream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "failed to read compute shader file" << std::endl;
    }

    const char* c_shader_code = compute_code.c_str();

    // compile shader
    unsigned int shader;

    shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &c_shader_code, NULL);
    glCompileShader(shader);

    // create program
    id = glCreateProgram();
    glAttachShader(id, shader);
    glLinkProgram(id);

    glUseProgram(id);
    // cleanup
    glDeleteShader(shader);

    // create input/output textures
    // Map 1
    glGenTextures(1, &mapOne);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mapOne);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1i(glGetUniformLocation(id, "mapOne"), 0);
    glUseProgram(0);
    glUseProgram(id);
    // Map 2
    glGenTextures(1, &mapTwo);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mapTwo);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1i(glGetUniformLocation(id, "mapTwo"), 1);

    glUseProgram(id);
    // Map 3
    glGenTextures(1, &mapThree);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mapThree);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1i(glGetUniformLocation(id, "mapThree"), 2);

    // Map 4
    glGenTextures(1, &mapFour);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mapFour);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1i(glGetUniformLocation(id, "mapFour"), 3);


    // Counter in buffer
    int counter = 0;
    GLuint ssbo;    // Shader storage buffer object
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(counter), &counter, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);  // unbind

    // Mu
    glGenTextures(1, &muMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, muMap);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1i(glGetUniformLocation(id, "muMap"), 5);

    // Rho
    glGenTextures(1, &rhoMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rhoMap);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1i(glGetUniformLocation(id, "rhoMap"), 5);
}

/* --- Use compute shader ---*/
void ComputeShader::use() 
{
    glUseProgram(id);
    glBindImageTexture(0, mapOne, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(1, mapTwo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
}

void ComputeShader::dispatch() {
    // 2D work group
    glDispatchCompute(work_size.x, work_size.y, 1);
}

void ComputeShader::wait()
{
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ComputeShader::set_values(std::vector<std::vector<float>>& values) 
{
    glActiveTexture(GL_TEXTURE0);
    // Compress into 1D vector
    unsigned int totalLenght = work_size.x * work_size.y;
    std::vector<float> data(totalLenght);
    auto it = data.begin();
    for (int i = 0; i < work_size.x; ++i)
    {
        std::copy(values[i].begin(), values[i].end(), it);
        it += work_size.y;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, work_size.x, work_size.y, 0, GL_RED, GL_FLOAT, data.data());
}

void ComputeShader::get_values(std::vector<std::vector<float>>& values)
{
    unsigned int collection_size = work_size.x * work_size.y;
    std::vector<float> compute_data(collection_size);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, compute_data.data());
    // Divide into 2D vector
    auto it = compute_data.begin();
    for (int i = 0; i < work_size.x; ++i)
    {
        std::copy(it, it + work_size.y, values[i].begin());
        it += work_size.y;
    }
}