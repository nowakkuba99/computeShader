#ifndef B0726C13_7480_403E_8594_FA5F9ABA905E
#define B0726C13_7480_403E_8594_FA5F9ABA905E
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* TO BE DONE: 
- make a singelton 
*/

/* Include openGL math functions */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Application
{
public:
    /* --Define functions for openGL-- */
    /* -Callback functions- */
    // Callback for resizing window
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    // Callback for user input
    void processInput(GLFWwindow *window);
    // Callback for mouse input
    //void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    /* Initialization functions */
    void initGLFW();
    int initGLAD();
};

#endif /* B0726C13_7480_403E_8594_FA5F9ABA905E */

