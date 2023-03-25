#ifndef E1898DCC_99E2_43F4_BB66_A8843F480BA1
#define E1898DCC_99E2_43F4_BB66_A8843F480BA1

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class ComputeShader {

    /**
     * Compute shader constructor.
     * 
     * Constructor reads shader code from file,
     * compiles shader, creating program and data
     * buffer through texture object.
     * 
     * @param path Path to compute shader code
     * @param size Work size group for compute shader
     * 
     * @return -
    */
    ComputeShader(const char* path, glm::uvec2 size);
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
    void set_values(float* values);

    /**
     * Get values from buffer texture object.
     * 
     * Function uses glGetTexImage and returns as vector of floats
     * 
     * @param -
     * 
     * @return Vector of float data from texture
    */
    std::vector<float> get_values();

private:
    glm::uvec2 work_size;
    unsigned int id;
    unsigned int out_tex;
};


#endif /* E1898DCC_99E2_43F4_BB66_A8843F480BA1 */
