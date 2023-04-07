// Standard includes
#include <iostream>
#include <cmath>

// OpenGL related includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Freetype libraries - text rendering
#include <ft2build.h>
#include FT_FREETYPE_H


// User includes
#include "../inc/app/application.hpp"
#include "../inc/shader/shader.hpp"
#include "../inc/shader/Textshader.hpp"
#include "../inc/computeShader/computeShader.hpp"


int main()
{

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //OpenGl does not support compute shaders on macos -> Translate to Metal API
#endif
    Application app;
    /* Initialize GLFW */
    app.initGLFW();

    /* Create window */
    GLFWwindow *window = glfwCreateWindow(800, 600, "LISA 2D SH Waves Solver", NULL, NULL); // Create window
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* Text shaders */
    TextShader textShader("shaderCodes/TextVertex.vs", "shaderCodes/TextFragment.fs");
    textShader.makeActive();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
    glUniformMatrix4fv(glGetUniformLocation(textShader.shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    textShader.initBuffers();
    /* Be ready for resize */
    glfwSetFramebufferSizeCallback(window, app.framebuffer_size_callback);

    /*
    // query limitations
    // -----------------
    int max_compute_work_group_count[3];
    int max_compute_work_group_size[3];
    int max_compute_work_group_invocations;

    for (int idx = 0; idx < 3; idx++) {
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &max_compute_work_group_count[idx]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &max_compute_work_group_size[idx]);
    }
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);

    std::cout << "OpenGL Limitations: " << std::endl;
    std::cout << "maximum number of work groups in X dimension " << max_compute_work_group_count[0] << std::endl;
    std::cout << "maximum number of work groups in Y dimension " << max_compute_work_group_count[1] << std::endl;
    std::cout << "maximum number of work groups in Z dimension " << max_compute_work_group_count[2] << std::endl;

    std::cout << "maximum size of a work group in X dimension " << max_compute_work_group_size[0] << std::endl;
    std::cout << "maximum size of a work group in Y dimension " << max_compute_work_group_size[1] << std::endl;
    std::cout << "maximum size of a work group in Z dimension " << max_compute_work_group_size[2] << std::endl;

    std::cout << "Number of invocations in a single local work group that may be dispatched to a compute shader " << max_compute_work_group_invocations << std::endl;
    */
    /* Compute shader creation */
    constexpr auto X_SIZE = 10;
    constexpr auto Y_SIZE = 10;
    ComputeShader compShader("shaderCodes/computeShader.comp", glm::uvec2(X_SIZE,Y_SIZE)); // One 2D texture
    compShader.use();
    
    // Create input vector -> With initial displacement
    std::vector<std::vector<float>> values(X_SIZE, std::vector<float>(Y_SIZE, 0));
    for (auto& vec : values)
    {
        vec[0] = 0;//1
    }
    compShader.set_values(values);
    std::cout<<"Created using OpenGL: " << glGetString(GL_VERSION) << std::endl;

    /* Vertex and Fragment shader creation */
    Shader shader("shaderCodes/vertexShader.vs", "shaderCodes/fragmentShader.fs");
    shader.makeActive();
    shader.setInt("tex", 0);

/* Main loop */
    /* Renders each frame with each iteration */
    int counter = 0;
    while (!glfwWindowShouldClose(window))
    {
        /* Input handling */
        app.processInput(window); // Get user input and process it

         /* Compute shader */
         compShader.use();
         compShader.dispatch();
         compShader.wait();
         //compShader.get_values(values, GL_TEXTURE0);
         //std::cout << "Map 0" << std::endl;
         ////for(const auto& vec: values)
         ////{
         ////    for (const auto& val : vec)
         ////    {
         ////        std::cout << val << " ";
         ////    }
         ////    std::cout << "\n";
         ////}
         //std::cout << values[0][0] << values[0][1] << "...\n";
         //std::cout << values[1][0] << values[1][1] << "...\n";
         //std::cout<<std::endl;
         //compShader.get_values(values, GL_TEXTURE0);
         //if (*(values[0].end() - 1) == 1000)
         //{
         //    glfwSetWindowShouldClose(window, true); // Set flag to close window
         //}

        /* Start rendering */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set color that will be set with clear command
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the depth buffer
        
        shader.makeActive();
        shader.setInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,compShader.getTexture());
        shader.renderQuad();
        std::string label = "Loop: " + std::to_string(counter) + "/ who knows?";
        textShader.RenderText(label , 2.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        textShader.RenderText("(C) Jakub Nowak 2023", 600.0f, 570.0f, 0.75f, glm::vec3(16/255, 16/255, 16/255));

        glfwSwapBuffers(window); // Swap current pixels values for the window
        glfwPollEvents(); // Check for events (e.g. keyboard interupts etc.) and calls callbacks
        //compShader.increase_map_counter();
        ++counter;
    }


    glfwTerminate(); // Clear/delete created objects
    return 0;
}