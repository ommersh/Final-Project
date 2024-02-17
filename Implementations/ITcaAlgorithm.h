#ifndef SHIELD_ITcaAlgorithm_H    // Check if the symbol SHIELD_ITcaAlgorithm_H is not defined
#define SHIELD_ITcaAlgorithm_H    // Define the symbol SHIELD_ITcaAlgorithm_H

#include "TCA_Calculation.h"

using namespace TCA_Calculation;

class ITcaAlgorithm
{
public:
	/// <summary>
	/// Get the TCA
	/// </summary>
	/// <param name="pointsInTime">The location and velocity of 2 objects in a set of points in time</param>
	/// <param name="timePoints">The points in time from 0 to n</param>
	/// <param name="lastPointIndex">The index of the last point in the arrays</param>
	/// <returns>The TCA</returns>
	virtual TCA runAlgorithm(sPointData* pointsInTime, double* timePoints, int lastPointIndex) = 0;


};
#endif //SHIELD_ITcaAlgorithm_H