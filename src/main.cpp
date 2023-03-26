// Standard includes
#include <iostream>
#include <cmath>

//OpenGL related includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include "../inc/app/application.hpp"
#include "../inc/shader/shader.hpp"
#include "../inc/computeShader/computeShader.hpp"

int main()
{

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    Application app;
    /* Initialize GLFW */
    app.initGLFW();

    /* Create window */
    GLFWwindow *window = glfwCreateWindow(800, 600, "openGL course", NULL, NULL); // Create window
    if (window == NULL)                                                           // Check if window properly created
    {
        std::cout << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Set the current window as current to render

    /* GLAD initialization */
    if (app.initGLAD() == -1)
    {
        return -1;
    }

    /* Be ready for resize */
    glfwSetFramebufferSizeCallback(window, app.framebuffer_size_callback);

    /* Compute shader creation */
    ComputeShader compShader("shaderCodes/computeShader.cm", glm::uvec2(10,1));
    // compShader.use();
    
    // float values[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // compShader.set_values(values);

    std::cout<<glGetString(GL_VERSION);

/* Main loop */
    /* Renders each frame with each iteration */
    while (!glfwWindowShouldClose(window))
    {
        /* Input handling */
        app.processInput(window); // Get user input and process it

        // /* Compute shader */
        // compShader.use();
        // compShader.dispatch();
        // compShader.wait();

        // auto data = compShader.get_values();
        // for(const auto& val: data)
        // {
        //     std::cout<<val<<" ";
        // }
        // std::cout<<std::endl;

        /* Start rendering */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set color that will be set with clear command
        glClear(GL_COLOR_BUFFER_BIT);         // Clear the current buffer with some color buffer

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the depth buffer

        glfwSwapBuffers(window); // Swap current pixels values for the window

        glfwPollEvents(); // Check for events (e.g. keyboard interupts etc.) and calls callbacks
    }

    

    glfwTerminate(); // Clear/delete created objects
    return 0;
}