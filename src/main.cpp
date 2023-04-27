// Standard includes
#include <iostream>
// User includes
#include "../inc/LISA_SH_Solver.h"

int main()
{
    /* Display banner */
    std::cout << banner << std::endl;

    /* SOLVER Settings */
    LISA_SH::solverSettings settings;
    settings.display = false;
    settings.specimenLength_meters = 400e-3;
    LISA_SH::Solver* solver = LISA_SH::Solver::getInstance(settings);

    solver->initSolver();
    solver->displayBasicInfo();
    solver->solve();

    delete solver;
    return 0;
}
/* Speed vs MATLAB for 400mm specimen

Display on
    Matlab: -
    C++:    ~22s
Display off
    Matlab: ~60s
    C++:    ~11s

*/

/* TO DO
- File writer class which will write 20 measurements data to 20 files and merge at the end (?)
- Add Correct compute shader loading based on settings
- Create hysteresis based compute shaders
- Create proper measurements system for compute shader
- Crack placement information passing to GPU
- Hysteresiss parameters passing to GPU -> Maybe second SSBO
*/