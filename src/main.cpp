// Standard includes
#include <iostream>
#include <cmath>
#include "../inc/LISA_SH_Solver.h"


int main()
{
    /* Display banner */
    std::cout << banner << std::endl;

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //OpenGl does not support compute shaders on macos -> Translate to Metal API
#endif

    constexpr auto X_SIZE = 4002;
    constexpr auto Y_SIZE = 22;

    /* SOLVER ELEMENTS */
    LISA_SH::solverSettings settings;
    LISA_SH::Solver* solver = LISA_SH::Solver::getInstance(settings,X_SIZE,Y_SIZE);

    solver->initSolver();

    solver->solve();

    delete solver;
    return 0;
}