#ifndef E1898DCC_99E2_43F4_BB66_A8843F480BA1
#define E1898DCC_99E2_43F4_BB66_A8843F480BA1

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../settings/settings.h"

class ComputeShader {
public:
    /**
     * Compute shader constructor.
     * 
     * @brief Constructor reads shader code from file,
     * compiles shader, creating program and data
     * buffer through texture object.
     * 
     * @param path:     Path to compute shader code
     * @param size:     Work size group for compute shader
     * 
     * @return -
    */
    ComputeShader(const char* path, glm::uvec2 size, LISA_SH::specimenType type);
    /**
     * Compute shader destructor.
     * 
     * Deletes program created while constucting compute
     * shader object.
     * 
     * @param -
     * 
     * @return -
    */
    ~ComputeShader() 
    {
        glDeleteProgram(id);
    }

    /**
     * Use compute shader.
     * 
     * Function activates and uses compute shader with 
     * created texture data.
     * 
     * @param -
     * 
     * @return -
    */
    void use();

    void useInelastic();

    void useElastic();

    /**
     * Dispatch compute shader.
     * 
     * Function dispatches 2D compute.
     * 
     * @param -
     * 
     * @return -
    */
    void dispatch();

    /**
     * Wait for end of computing all values.
     * 
     * Function uses glMemoryBarier to wait for end
     * of computing.
     * 
     * @param -
     * 
     * @return -
    */
    void wait();
    
    /**
     * Set values for buffer texture object.
     * 
     * Function uses glTexImage2D with float data provided
     * by user.
     * 
     * @param values Pointer to float array of input values
     * 
     * @return -
    */
    void set_values(std::vector<std::vector<float>>& values, GLenum texture);

    /**
     * Get values from buffer texture object.
     * 
     * Function uses glGetTexImage and returns as vector of floats
     * 
     * @param -
     * 
     * @return Vector of float data from texture
    */
    void get_values(std::vector<std::vector<float>>& values, GLenum texture);

    /**
     * Handles SSBO managemet.
     *
     * Function updates counter, updates extortion, reads measurements data etc.
     *
     * @param -
     *
     * @return Vector of measured values along the top of the simulated plate.
    */
    auto update_ssbo(float extortionValue) -> std::vector<float>;
    // Hysteresis params setter
    auto setOtherParams(const LISA_SH::hysteresis_params_ssbo& params) -> void;     //CHANGE STRUCT TO CUSTOM

private:
    // SSBO managemnt helper functions
    // Increase the counter from 0 up to 3 and reapeat
    auto increase_map_counter() -> void;
    // Create vector to return from update function
    auto createVectorFromArray(std::vector<float>& vecToSave) const -> void;

public:
    /**
     * Loads and sets correct textures for display pourposes.
     *
     * Based on counter, used by shader function activates texture that is a
     * current displacement map.
     *
     * @param -
     *
     * @return Active texture number to be set by display shader.
    */
    auto getTexture() const -> unsigned int;

private:
    glm::uvec2 work_size;
    unsigned int id;
    // Displacement maps
    unsigned int mapOne;
    unsigned int mapTwo;
    unsigned int mapThree;
    unsigned int mapFour;
    // Shader storage buffer
    struct shader_storage_buffer
    {
        int     count = 0;
        float   measurements[20];
        float   extortion = 0;
        int     extortionPlacement = 0;

        shader_storage_buffer()
        {
            for (size_t it = 0; it < 20; ++it)
            {
                measurements[it] = 0;
            }
        }
    } shaderStorageBuffer;
    // Hysteresis params
    LISA_SH::hysteresis_params_ssbo hysteresisParams;
    // Shader Storage Buffer Objects
    GLuint ssbo;
    GLuint ssbo2;
    // Parameter maps
    unsigned int muMap;
    unsigned int rhoMap;
    // Inealstic hysteresis params maps
    unsigned int hysteresisMap;
};


#endif /* E1898DCC_99E2_43F4_BB66_A8843F480BA1 */
