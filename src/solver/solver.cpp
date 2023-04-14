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

#include "../../inc/solver/solver.hpp"
namespace LISA_SH
{
/* --------- Singelton instance getter ---------*/
Solver* Solver::m_Solver = nullptr;
Solver* Solver::getInstance(solverSettings set)
{
    if(m_Solver == nullptr)
        m_Solver = new Solver(set);
    return m_Solver;
}
/* --------- Initialize function ---------*/
void Solver::initSolver()
{
    /* --------- Initialize displacement vectors ---------*/
    // Previous
    p_PrevDisplacement = std::make_unique<std::vector<std::vector<float>>>
    (std::vector<std::vector<float>>(m_GridSet.xGridSize,std::vector<float>(m_GridSet.yGridSize,0.0f)));
    // Current
    p_CurrDisplacement = std::make_unique<std::vector<std::vector<float>>>
    (std::vector<std::vector<float>>(m_GridSet.xGridSize,std::vector<float>(m_GridSet.yGridSize,0.0f)));
    // Next
    p_NextDisplacement = std::make_unique<std::vector<std::vector<float>>>
    (std::vector<std::vector<float>>(m_GridSet.xGridSize,std::vector<float>(m_GridSet.yGridSize,0.0f)));
    
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
            return;
        }
    }
    
//    std::vector<float> timeVec(30000,0.0f);
//    for(int i = 1; i<timeVec.size(); ++i)
//    {
//        timeVec[i] = timeVec[i-1]+m_TimeSet.dt;
//    }
//    matplot::plot(timeVec, *p_Extortion.get());
//    matplot::show();
}

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
}

void Solver::initExtortionWaveMix(unsigned int length)
{
    struct timeVec
    {
        float currTime;
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
}

void Solver::solve()
{
    if(m_Settings.crack.crack == crackDefinition::crackPlacement::linearSpecimen)
    {
        for(unsigned int time = 0; time < m_TimeSet.numberOfTimeSteps; ++time)
        {
            for(unsigned int x = 0; x < m_GridSet.xGridSize; ++x)
            {
                for(unsigned int y = 0; y < m_GridSet.yGridSize; ++y)
                {
                    (*(p_NextDisplacement.get()))[x][y] = 0;
//                    wNext(x,y) = 2*wCurrent(x,y) - wPrevious(x,y) + ...
//                                     2.*(dtSquared./(rhoCell(x,y)+rhoCell(x-1,y)+rhoCell(x-1,y-1)+rhoCell(x,y-1))).*((...
//                                     wCurrent(x+1,y).*(muCell(x,y) + muCell(x,y-1)) + wCurrent(x-1,y).*(muCell(x-1,y) + muCell(x-1,y-1)) -...
//                                     wCurrent(x,y).*(muCell(x,y) + muCell(x-1,y) + muCell(x-1,y-1) + muCell(x,y-1)))/dxSquared +...
//                                     (wCurrent(x,y+1).*(muCell(x,y) + muCell(x-1,y)) + wCurrent(x,y-1).*(muCell(x-1,y-1) + muCell(x,y-1)) -...
//                                     wCurrent(x,y).*(muCell(x,y) + muCell(x-1,y) + muCell(x-1,y-1) + muCell(x,y-1)))/dySquared);
                }
            }
//            std::vector<float> X(m_GridSet.xGridSize), Y(m_GridSet.yGridSize);
//            for(unsigned int i = 1; i < m_GridSet.xGridSize; ++i)
//            {
//                X[i] = X[i-1] + m_GridSet.dx;
//            }
//            for(unsigned int i = 1; i < m_GridSet.yGridSize; ++i)
//            {
//                Y[i] = Y[i-1] + m_GridSet.dy;
//            }
        }
    }
}




}   //LISA_SH namespace
