//
//  settings.h
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 12/03/2023.
//

#ifndef settings_h
#define settings_h
// Includes
#include <cstdint>
// User includes
#include "../functions/functions.h"

namespace LISA_SH
{
struct extortionDefinition
{
    enum class extortionType : uint8_t
    {
        singleSin,
        waveMix
    };
    
    enum class extortionSource: uint8_t
    {
        pack,
        constant
    };
    
    extortionType type              = extortionType::singleSin;
    float singleFrequency           = 1.75e5;   // 175 kHz
    float mixFrequencyOne           = 68e3;     // 68 kHz
    float mixFrequencyTwo           = 285e3;    // 285 kHz
    extortionSource source          = extortionSource::pack;
    int numOfPeriods                = 14;
    float amplitude                 = 1e-6;
};

struct hysteresisParams
{
    enum class hysteresisType : uint8_t
    {
        elasitc,
        inelastic
    };
    hysteresisType type             = hysteresisType::inelastic;
    float emPositive_m              = 1e-6;
    float emNegative_m              = 1e-6;
    // Inelastic parameters
    int beta1                       = 100;
    int beta2                       = 100;
    int alpha                       = 100;
    // Elastic parameters
    int gamma1                      = 100;
    int gamma2                      = 100;
    int gamma3                      = 100;
    int gamma4                      = 100;
    
};

struct crackDefinition
{
    enum class crackPlacement : uint8_t
    {
        singleCrack,
        linearSpecimen,
        nonlinearSpecimen
    };
    crackPlacement crack            = crackPlacement::linearSpecimen;
    float crackPosition_meters      = 400e-3;   // 400 mm
    float crackLength_meters        = 10e-3;    // 10mm
};
struct solverSettings
{
    /* SPECIMEN SIZE */
    float specimenLength_meters     = 800e-3;    // 800 mm
    float specimenThickness_meters  = 2e-3;      // 2 mm
    
    /* SPECIMEN MATERIAL */
    float youngModulus_pa           = 68.9e9;    // 68.9 GPa
    float density_kg_m3             = 2700;      // 2700 kg/m3
    float poisson                   = 0.3;
    
    /* SPACE DISCRETIZATION */
    float dx_meters                 = 1e-4;      // 0.1 mm
    float dy_meters                 = 1e-4;      // 0.1 mm
    
    /* TIME DISCRETIZATION */
    float dt_seconds                = 1e-8;      // 0.01 us
    float simulationTime_seconds    = 30e-5;     // 0.3 ms
    
    /* EXTORTION */
    extortionDefinition extortion   = extortionDefinition();
    
    /* HYSTEREIS PARAMETERS */
    hysteresisParams hysteresisMode = hysteresisParams();
    
    /* CRACK */
    crackDefinition crack           = crackDefinition();

    /* DISPLAY SETTINGS */
    bool display                    = true;
};
/* STRUCTURES USED BY SOLVER INITIALIZED BASED ON SETTINGS STRUCTURE */
struct grid
{
    /* SPECIMEN RELATED */
    float dx;
    float dy;
    
    float dxSquare;
    float dySquare;
    
    float dxCube;
    float dyCube;
    
    unsigned int xGridSize;
    unsigned int yGridSize;
    
    //Constructor
    inline grid(float dx_meters, float dy_meters, float xLength_meters, float yLength_meters)
    : 
        dx(dx_meters), dy(dy_meters),
        dxSquare(dx_meters*dx_meters), dySquare(dy_meters*dy_meters),
        dxCube(dx_meters*dx_meters*dx_meters), dyCube(dy_meters*dy_meters*dy_meters),
        xGridSize(xLength_meters/dx_meters + 2), yGridSize(yLength_meters/dy_meters + 2)
    {};
};
struct material
{
    float rho;
    float mu;   //Kirchoff's shear modulus
    
    //Constructor
    inline material(float youngModulus_Pa, float poisson, float density_kg_m3)
    :
        rho(density_kg_m3),
        mu(youngModulus_Pa/(2*(1+poisson)))
    {};
};
struct time
{
    float dt;
    float dtSquared;
    float simulationTime;
    unsigned int numberOfTimeSteps;
    
    //Constructor
    inline time(float dt_seconds, float simulationTime_seconds)
    :
        dt(dt_seconds),
        dtSquared(dt_seconds*dt_seconds), 
        simulationTime(simulationTime_seconds),
        numberOfTimeSteps(simulationTime_seconds/dt_seconds)
    {};
};

}   // LISA_SH namespace
#endif /* settings_h */
