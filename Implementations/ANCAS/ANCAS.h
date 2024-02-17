#ifndef SHIELD_ANCAS_H    // Check if the symbol SHIELD_ANCAS_H is not defined
#define SHIELD_ANCAS_H    // Define the symbol SHIELD_ANCAS_H

#include <limits>
#include <math.h>
#include <cmath>
#include "ITcaAlgorithm.h"

#ifndef M_PI
const double M_PI = 2 * acos(0.0);
#endif
using namespace TCA_Calculation;

/// <summary>
/// Implementation of Cubic polynomial for ANCAS
/// </summary>
class ANCASCubicPolynomial
{
public:
	double coefficients[4];
	void createCoefficients(double f[4], double Tau[4]);
	double getValue(double x);
};
/// <summary>
/// Implentation of ANCAS(Alfano\Negron Close Approach Software) - based on Determining Satellite Close Approaches,Part 2 by Salvatore Alfano
/// 
/// </summary>
class ANCAS : public ITcaAlgorithm
{
public:
	TCA runAlgorithm(sPointData* pointsInTime, double* timePoints, int lastPointIndex);
	virtual void calculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots);

protected:
	virtual int getRootsInInterval(ANCASCubicPolynomial P, double result[3]);

};



#endif //SHIELD_ANCAS_H