#ifndef SHIELD_ITcaAlgorithm_H    // Check if the symbol SHIELD_ITcaAlgorithm_H is not defined
#define SHIELD_ITcaAlgorithm_H    // Define the symbol SHIELD_ITcaAlgorithm_H

#include "TCA_Calculation.h"

using namespace TCA_Calculation;

class ITcaAlgorithm
{
public:
	virtual TCA runAlgorithm(sPointData* pointsInTime, double* timePoints, int lastPointIndex) = 0;


};
#endif //SHIELD_ITcaAlgorithm_H