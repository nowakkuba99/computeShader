//
//  main.cpp
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 12/03/2023.
//
/* Libraries includes */
#include <iostream>
#include <vector>
#include <cmath>
#include <matplot/matplot.h>
/* User includes */
#include "../inc/LISA_SH_Solver.h"

int main(int argc, const char * argv[]) {
    /* Add brew to path */
    addBrewToPATH();

    //std::cout << banner<<"\n";
    LISA_SH::solverSettings settings;
    LISA_SH::Solver* solver = LISA_SH::Solver::getInstance(settings);
    
    solver->initSolver();
    
    solver->solve();
    
    delete solver;
//    auto [x, y, z] = matplot::peaks(20);
//
//    matplot::surf(x, y, z);
//    std::cout.setstate(std::ios::failbit);
//    matplot::show();
//    std::cout.clear() ;
    

    return 0;
}
