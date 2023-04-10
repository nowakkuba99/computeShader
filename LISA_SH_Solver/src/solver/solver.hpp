//
//  solver.hpp
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 12/03/2023.
//

#ifndef solver_hpp
#define solver_hpp

#include <vector>

#include "../settings/settings.h"
namespace LISA_SH
{
class Solver
{
protected:
    // Protected constructor
    inline Solver(solverSettings set)
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
    // Singelton getter
    static Solver* getInstance(solverSettings set);
    
    /* ---------------------------------------------------------------------------- */
    /* ---------------------SPECIMEN INITIALIZATION FUNCTIONS---------------------- */
    /* ---------------------------------------------------------------------------- */
    /**
        Function used to initialize solver elemets such as displacement arrays, constants etc.
     */
    void initSolver();
    /**
        Function used to start solver operation
     */
    void solve();
protected:
    /* ---------------------------------------------------------------------------- */
    /* ---------------------------SOlVER FUNCTIONS--------------------------------- */
    /* ---------------------------------------------------------------------------- */
    /**
        Function used to solve the current state of grid for linear definition
    
        @param displacementPrevious The previous state of grid
        @param displacementCurrent   The current state of grid
        @param displacementNext          The address of buffet to wrtie to
     */
    void solveLinear(
               std::shared_ptr<std::vector<std::vector<float>>> &displacementPrevious,
               std::shared_ptr<std::vector<std::vector<float>>> &displacementCurrent,
               std::shared_ptr<std::vector<std::vector<float>>> &displacementNext);
    /**
        Function used to solve the current state of grid for linear definition with elastic hysteresis
    
        @param displacementPrevious The previous state of grid
        @param displacementCurrent   The current state of grid
        @param displacementNext          The address of buffet to wrtie to
     */
    void solveNonlinearHysteresis_Elastic(
               std::shared_ptr<std::vector<std::vector<float>>> &displacementPrevious,
               std::shared_ptr<std::vector<std::vector<float>>> &displacementCurrent,
               std::shared_ptr<std::vector<std::vector<float>>> &displacementNext);
    /**
        Function used to solve the current state of grid for nonlinear definition with inelastic hysteresis
    
        @param displacementPrevious The previous state of grid
        @param displacementCurrent   The current state of grid
        @param displacementNext          The address of buffet to wrtie to
     */
    void solveNonlinearHysteresis_Inelastic(
               std::shared_ptr<std::vector<std::vector<float>>> &displacementPrevious,
               std::shared_ptr<std::vector<std::vector<float>>> &displacementCurrent,
               std::shared_ptr<std::vector<std::vector<float>>> &displacementNext);
    

private:
    // Member variables settings
    solverSettings  m_Settings;
    time            m_TimeSet;
    material        m_MaterialSet;
    grid            m_GridSet;
    // Displacement member variables
    std::unique_ptr<std::vector<std::vector<float>>>            p_PrevDisplacement;
    std::unique_ptr<std::vector<std::vector<float>>>            p_CurrDisplacement;
    std::unique_ptr<std::vector<std::vector<float>>>            p_NextDisplacement;
    // Distortion variable
    std::unique_ptr<std::vector<float>>                         p_Extortion;
    
    /* PRIVATE FUNCTIONS */
    void initExtortionSingleSin(unsigned int length);
    void initExtortionWaveMix(unsigned int length);
};
}   //LISA_SH namespace
#endif /* solver_hpp */
