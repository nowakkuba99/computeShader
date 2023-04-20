// Standard includes
#include <iostream>
#include "../inc/LISA_SH_Solver.h"


int main()
{
    /* Display banner */
    std::cout << banner << std::endl;

    constexpr auto X_SIZE = 4002;
    constexpr auto Y_SIZE = 22;

    /* SOLVER Settings */
    LISA_SH::solverSettings settings;
    settings.display = false;
    LISA_SH::Solver* solver = LISA_SH::Solver::getInstance(settings,X_SIZE,Y_SIZE);

    solver->initSolver();

    solver->solve();

    delete solver;
    return 0;
}