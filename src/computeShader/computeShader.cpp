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
    glUniform1f(glGetUniformLocation(id, "mapOne"), 0);
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
    glUniform1f(glGetUniformLocation(id, "mapTwo"), 1);

    glUseProgram(id);
    // Map 3
    glGenTextures(1, &mapThree);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mapThree);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1f(glGetUniformLocation(id, "mapThree"), 2);

    // Map 4
    glGenTextures(1, &mapFour);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mapFour);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1f(glGetUniformLocation(id, "mapFour"), 3);

    // Mu
    glGenTextures(1, &muMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, muMap);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1f(glGetUniformLocation(id, "muMap"), 4);

    // Rho
    glGenTextures(1, &rhoMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, rhoMap);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, NULL);
    glUniform1f(glGetUniformLocation(id, "rhoMap"), 5);



    // Counter in buffer
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shaderStorageBuffer), &shaderStorageBuffer, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);  // unbind

}

/* --- Use compute shader ---*/
void ComputeShader::use() 
{
    glUseProgram(id);
    glBindImageTexture(0, mapOne, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(1, mapTwo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(2, mapThree, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(3, mapFour, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(4, muMap, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
    glBindImageTexture(5, rhoMap, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);

}

void ComputeShader::dispatch() {
    // 2D work group
    glDispatchCompute(work_size.x/174, work_size.y/2, 1);
}

void ComputeShader::wait()
{
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ComputeShader::set_values(std::vector<std::vector<float>>& values, GLenum texture)
{
    glActiveTexture(texture);
    // Compress into 1D vector
    unsigned int totalLenght = work_size.x * work_size.y;
    std::vector<float> data(totalLenght);
    auto it = data.begin();
    for (int i = 0; i < work_size.y; ++i)
    {
        std::copy(values[i].begin(), values[i].end(), it);
        it += work_size.x;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, work_size.x, work_size.y, 0, GL_RED, GL_FLOAT, data.data());
}

void ComputeShader::get_values(std::vector<std::vector<float>>& values, GLenum texture)
{
    glActiveTexture(texture);
    unsigned int collection_size = work_size.x * work_size.y;
    std::vector<float> compute_data(collection_size);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, compute_data.data());
    // Divide into 2D vector
    auto it = compute_data.begin();
    for (int i = 0; i < work_size.y; ++i)
    {
        std::copy(it, it + work_size.x, values[i].begin());
        it += work_size.x;
    }
}
_NODISCARD
auto ComputeShader::update_ssbo(float extortionValue) -> std::vector<float>
{
    // Get handle to the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    GLvoid* ssboHandle = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    // Read data from SSBO
    memcpy(&shaderStorageBuffer, ssboHandle, sizeof(shaderStorageBuffer));
    std::vector<float> measurementsData(20);
    createVectorFromArray(measurementsData);
    // Prepare new values to write to SSBO
    increase_map_counter();
    shaderStorageBuffer.extortionPlacement = 15;
    shaderStorageBuffer.extortion = extortionValue;
    // Write data to SSBO
    memcpy(ssboHandle, &shaderStorageBuffer, sizeof(shaderStorageBuffer));
    // Delete handle to SSBO
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    return measurementsData;
}

auto ComputeShader::increase_map_counter() -> void
{
    if (shaderStorageBuffer.count < 3)
    {
        ++shaderStorageBuffer.count;
    }
    else
    {
        shaderStorageBuffer.count = 0;
    }
}
inline auto ComputeShader::createVectorFromArray(std::vector<float>& vecToSave) const -> void
{
    std::copy(&shaderStorageBuffer.measurements[0], &shaderStorageBuffer.measurements[20], vecToSave.begin());
}
auto ComputeShader::getTexture() const -> unsigned int
{
    // Get the handle to the current displacement map
    if (shaderStorageBuffer.count == 0)
    {
        glActiveTexture(GL_TEXTURE1);   // Map2
        glBindTexture(GL_TEXTURE_2D, mapTwo);
        return 1;
    }
    else if (shaderStorageBuffer.count == 1)
    {
        glActiveTexture(GL_TEXTURE2);   // Map3
        glBindTexture(GL_TEXTURE_2D, mapThree);
        return 2;
    }
    else if (shaderStorageBuffer.count == 2)
    {
        glActiveTexture(GL_TEXTURE3);   // Map4
        glBindTexture(GL_TEXTURE_2D, mapFour);
        return 3;
    }
    else if (shaderStorageBuffer.count == 3)
    {
        glActiveTexture(GL_TEXTURE0);   // Map1
        glBindTexture(GL_TEXTURE_2D, mapOne);
        return 0;
    }
    else
    {
        std::cerr << "Unknown counter value!\n";
        return 0;
    }
}
// COUNTER READ/WRITE RULES
/*
        --- Count = 0 ---
        1: Previous -> READ ONLY
        2: Current -> READ ONLY
        3: Next -> WRITE
        4: Unused
        --- Count = 1 ---
        1: Unused
        2: Previous -> READ ONLY
        3: Current -> READ ONLY
        4: Next -> WRITE
        --- Count = 2 ---
        1: Next -> WRITE
        2: Unused
        3: Previous -> READ ONLY
        4: Current - READ ONLY
        --- Count = 3 ---
        1: Current -> READ ONLY
        2: Next -> WRITE
        3: Unused
        4: Previous -> READ ONLY
        --- REPEAT ---
*/