//
//  functions.h
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 13/03/2023.
//

#ifndef functions_h
#define functions_h

#include <vector>
#include <math.h>

template <typename T>
void hannWindowMultiply(std::vector<T>& inputData)
{
    const unsigned int length = inputData.length();
    /* HANNING WINDOW */
    for(int i = 0; i < length; i++)
    {
        inputData[i] *= (0.5 * (1 - cos(2*M_PI*i/length)));
    }
}

template <typename T>
std::vector<T> hannWindow(const unsigned int length)
{
    std::vector<T> hann(length);
    /* HANNING WINDOW */
    for(int i = 0; i < length; i++)
    {
        hann[i] = (0.5 * (1 - cos(2*M_PI*i/length)));
    }
    return hann;
}

#endif /* functions_h */
