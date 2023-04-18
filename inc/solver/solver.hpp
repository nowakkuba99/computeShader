//
//  solver.hpp
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 12/03/2023.
//

#ifndef solver_hpp
#define solver_hpp

#include <vector>


// OpenGL related includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Freetype libraries - text rendering
#include <ft2build.h>
#include FT_FREETYPE_H

// User includes
#include "../app/application.hpp"
#include "../shader/shader.hpp"
#include "../shader/Textshader.hpp"
#include "../computeShader/computeShader.hpp"
#include "../settings/settings.h"

namespace LISA_SH
{
class Solver
{
protected:
    // Protected constructor
    inline Solver(solverSettings set, const int X_SIZE, const int Y_SIZE)
    :
    m_GridSet(set.dx_meters,set.dy_meters,set.specimenLength_meters, set.specimenThickness_meters),
    m_TimeSet(set.dt_seconds, set.simulationTime_seconds),
    m_MaterialSet(set.youngModulus_pa, set.poisson, set.density_kg_m3),
    m_Settings(set)
    {
    };
    Solver() = delete;
    Solver(const Solver&) = delete;
    void operator=(const Solver&) = delete;
    // Instance
    static Solver*         m_Solver;
public:
    ~Solver()
    {
        if (p_TextShader != nullptr)
            delete p_TextShader;
        if (p_CompShader != nullptr)
            delete p_CompShader;
        if (p_Shader != nullptr)
            delete p_Shader;
    }
    // Singelton getter
    static Solver* getInstance(solverSettings set, const int X_SIZE, const int Y_SIZE);
    
    /* ---------------------------------------------------------------------------- */
    /* ---------------------SPECIMEN INITIALIZATION FUNCTIONS---------------------- */
    /* ---------------------------------------------------------------------------- */
    /**
        Function used to initialize solver elemets such as displacement arrays, constants etc.
     */
    auto initSolver() -> int;

    /**
    *   Function used to start solver operation
    *   @param: -
    *   @return: -
    */
    void solve();
protected:
    /* ---------------------------------------------------------------------------- */
    /* ---------------------------SOlVER FUNCTIONS--------------------------------- */
    /* ---------------------------------------------------------------------------- */
    
    /**
    *    Function used to solve the current state of grid for linear definition
    *    @brief
    *    @param: -
    *    @return: -
    */
    void solveLinear();
    /**
        Function used to solve the current state of grid for linear definition with elastic hysteresis
     */
    void solveNonlinearHysteresis_Elastic();
    /**
        Function used to solve the current state of grid for nonlinear definition with inelastic hysteresis
     */
    void solveNonlinearHysteresis_Inelastic();
    

private:
    // Member variables settings
public:
    solverSettings  m_Settings;
    time            m_TimeSet;
    material        m_MaterialSet;
    grid            m_GridSet;
    int             m_X_SIZE;
    int             m_Y_SIZE;
    // Distortion variable
public:
    std::unique_ptr<std::vector<float>> p_Extortion;
    // OpenGL related
    Application                     m_App;
    GLFWwindow*                     p_Window        = nullptr;
    TextShader*                     p_TextShader    = nullptr;
    ComputeShader*                  p_CompShader    = nullptr;
    Shader*                         p_Shader        = nullptr;
private:
    /* PRIVATE FUNCTIONS */
    /* Extortion */
    void initExtortionSingleSin(unsigned int length);
    void initExtortionWaveMix(unsigned int length);
    /* Compute shader and display */
    void initOpenGL();
};
}   //LISA_SH namespace
#endif /* solver_hpp */
