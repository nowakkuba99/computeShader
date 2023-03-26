#include <iostream>

#include "../../inc/app/application.hpp"



/* OpenGL specific function bodies */
/* ----------------------------------- */
/* Resize buffer size while window size changed */
void Application::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // Set ViewPort to proper values
}

/* Process user input given by key presses */
void Application::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If ESC pressed
    {
        glfwSetWindowShouldClose(window, true); // Set flag to close window
    }
}
//void openGL::mouseCallback(GLFWwindow *window, double xpos, double ypos);

/* Initliazie OpenGL v. 3.3 */
void Application::initGLFW()
{
    /* Initialize GLFW */
    glfwInit();                                                    // Initialize GLFW library
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                 // Set openGL version to 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);                 // Set openGL version to 4.5
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set openGL profile to core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Set for MAC OS to set window forward
}

/* Initliaze GLAD */
int Application::initGLAD()
{
    /* GLAD initialization */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    return 0;
}