//
//  solver.cpp
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 12/03/2023.
//
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <algorithm>

// Images loader
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "../../inc/stb/stb_image.h"

#include "../../inc/solver/solver.hpp"
namespace LISA_SH
{
/* --------- Singelton instance getter ---------*/
Solver* Solver::m_Solver = nullptr;
Solver* Solver::getInstance(solverSettings set, const int X_SIZE, const int Y_SIZE)
{
    if(m_Solver == nullptr)
        m_Solver = new Solver(set,X_SIZE, Y_SIZE);
    m_Solver->m_X_SIZE = X_SIZE;
    m_Solver->m_Y_SIZE = Y_SIZE;
    return m_Solver;
}
/* --------- Initialize function ---------*/
auto Solver::initSolver() -> int
{
    /* --------- Initialize extortion ---------*/
    p_Extortion = std::make_unique<std::vector<float>>
    (std::vector<float>(m_TimeSet.numberOfTimeSteps, 0.0f));
    unsigned int length = m_TimeSet.numberOfTimeSteps;
    if(m_Settings.extortion.source == extortionDefinition::extortionSource::pack)
    {
        if(m_Settings.extortion.type == extortionDefinition::extortionType::singleSin)
        {
            length = ((1/m_Settings.extortion.singleFrequency) * m_Settings.extortion.numOfPeriods)/m_TimeSet.dt;
            initExtortionSingleSin(length);
            //Multiply though hanning window
            std::vector<float> hann = hannWindow<float>(length);
            std::transform(
                           (*p_Extortion.get()).begin(), (*p_Extortion.get()).begin()+length,
                           hann.begin(),
                           (*p_Extortion.get()).begin(), std::multiplies<float>());
        }
        else if(m_Settings.extortion.type == extortionDefinition::extortionType::waveMix)
        {
            length = ((1/m_Settings.extortion.singleFrequency) * m_Settings.extortion.numOfPeriods)/m_TimeSet.dt;
            initExtortionWaveMix(length);
            //Multiply though hanning window
            std::vector<float> hann = hannWindow<float>(length);
            std::transform(
                           (*p_Extortion.get()).begin(), (*p_Extortion.get()).begin()+length,
                           hann.begin(),
                           (*p_Extortion.get()).begin(), std::multiplies<float>());
        }
        else
        {
            std::cerr<<"Wrong extortion definition! \n";
            return -1;
        }
    }
    /* --------- Initialize OpenGL ---------*/
    m_App.initGLFW();

    /* Create window */
    p_Window = glfwCreateWindow(800, 600, "LISA 2D SH Waves Solver", NULL, NULL); // Create window
    if (p_Window == NULL)                                                           // Check if window properly created
    {
        std::cout << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(p_Window); // Set the current window as current to render

    /* Load app icon */
    GLFWimage images[1];
    images[0].pixels = stbi_load("C:/Users/nowak/source/repos/ComputeShader_LISA/images/agh.jpg", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(p_Window, 1, images);
    stbi_image_free(images[0].pixels);

    /* GLAD initialization */
    if (m_App.initGLAD() == -1)
    {
        return -1;
    }
    /* Text shaders */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    p_TextShader = new TextShader("shaderCodes/TextVertex.vs", "shaderCodes/TextFragment.fs");
    p_TextShader->makeActive();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
    glUniformMatrix4fv(glGetUniformLocation(p_TextShader->shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    p_TextShader->initBuffers();
    /* Be ready for resize */
    glfwSetFramebufferSizeCallback(p_Window, m_App.framebuffer_size_callback);

    /* Activate compute shader */
    p_CompShader = new ComputeShader("shaderCodes/computeShader.comp", glm::uvec2(m_X_SIZE, m_Y_SIZE));
    p_CompShader->use();

    /* Vertex and Fragment shader creation for display pourposes */
    p_Shader =  new Shader("shaderCodes/vertexShader.vs", "shaderCodes/fragmentShader.fs");
    p_Shader->makeActive();
    p_Shader->setInt("tex", 0);

    // Create input vectors
    // Mu map
    std::vector<std::vector<float>> values(m_Y_SIZE, std::vector<float>(m_X_SIZE, 0));
    for (size_t row = 1; row < m_Y_SIZE - 2; ++row)
    {
        for (size_t col = 1; col < m_X_SIZE - 2; ++col)
        {
            values[row][col] = m_MaterialSet.mu;
        }
    }
    p_CompShader->set_values(values, GL_TEXTURE4);
    values.clear();
    values = std::vector<std::vector<float>>(m_Y_SIZE, std::vector<float>(m_X_SIZE, 0));
    // Rho map
    for (size_t row = 1; row < m_Y_SIZE - 2; ++row)
    {
        for (size_t col = 1; col < m_X_SIZE - 2; ++col)
        {
            values[row][col] = m_MaterialSet.rho;
        }
    }
    p_CompShader->set_values(values, GL_TEXTURE5);


    return 0;
} // Init solver

void Solver::initExtortionSingleSin(unsigned int length)
{
    struct timeVec
    {
        float currTime = 0;
        float dt;
        
        timeVec(float dt_s) : dt(dt_s) {};
        
        timeVec& operator++()
        {
            currTime += dt;
            return *this;
        }
    };
    timeVec timeValue(m_TimeSet.dt);
    for(auto it = (*p_Extortion.get()).begin(); it != (*p_Extortion.get()).begin()+length; it++)
    {
        *it = m_Settings.extortion.amplitude * sin(2*M_PI*m_Settings.extortion.singleFrequency*timeValue.currTime);
        ++timeValue;
    }
} // InitExtortionSingleSin

void Solver::initExtortionWaveMix(unsigned int length)
{
    struct timeVec
    {
        float currTime = 0;
        float dt;
        
        timeVec(float dt_s) : dt(dt_s) {};
        
        timeVec& operator++()
        {
            currTime += dt;
            return *this;
        }
    };
    timeVec timeValue(m_TimeSet.dt);
    for(auto it = (*p_Extortion.get()).begin(); it != (*p_Extortion.get()).begin()+length; it++)
    {
        *it =  m_Settings.extortion.amplitude * sin(2*M_PI*m_Settings.extortion.mixFrequencyTwo*timeValue.currTime);
        ++timeValue;
    }
} // InitExtortionWaveMix

void Solver::solve()
{
    auto it = p_Extortion->begin();
    /* Main loop */
    std::cout << "Measure time for 10 000 iterations\n";
    auto startTime = glfwGetTime();
    std::cout << "Start time: " << startTime << "\n";
    /* Renders each frame with each iteration */
    int counter = 0;
    while (!glfwWindowShouldClose(p_Window) && counter < 10000)
    {
        /* Input handling */
        m_App.processInput(p_Window); // Get user input and process it

        /* Compute shader */
        p_CompShader->use();
        p_CompShader->dispatch();
        p_CompShader->wait();

        /* Start rendering */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set color that will be set with clear command
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the depth buffer

        /* Active correct map based on counter */
        p_Shader->makeActive();
        int currentDisplacementTexture = p_CompShader->getTexture();
        p_Shader->setInt("tex", currentDisplacementTexture);
        /* Render displacement map */
        p_Shader->renderQuad();
        /* Render text information */
        std::string label = "Loop: " + std::to_string(counter) + "/ who knows?";
        p_TextShader->RenderText(label, 2.0f, 10.0f, 1.0f, glm::vec3(16 / 255, 16 / 255, 16 / 255));
        p_TextShader->RenderText("(C) Jakub Nowak 2023", 600.0f, 580.0f, 0.75f, glm::vec3(16 / 255, 16 / 255, 16 / 255));

        /* OpenGL buffer swap and event poll */
        glfwSwapBuffers(p_Window); // Swap current pixels values for the window
        glfwPollEvents(); // Check for events (e.g. keyboard interupts etc.) and calls callbacks

        /* Counters incrementation */
        p_CompShader->update_ssbo(*it);
        ++it;
        ++counter;
    }
    std::cout << "End time: " << glfwGetTime() - startTime << "\n";

    glfwTerminate(); // Clear/delete created objects
}




}   //LISA_SH namespace
