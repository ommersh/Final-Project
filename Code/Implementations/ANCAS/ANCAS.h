#ifndef SHIELD_ANCAS_H    // Check if the symbol SHIELD_ANCAS_H is not defined
#define SHIELD_ANCAS_H    // Define the symbol SHIELD_ANCAS_H

#include <limits>
#include <math.h>
#include <cmath>
#include "ITcaAlgorithm.h"

#ifndef M_PI
const double M_PI = 2 * acos(0.0);
#endif
using namespace TcaCalculation;

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
	virtual ~ANCAS()
	{

	};
	TCA runAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex);
	virtual void calculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots);

protected:
	TCA ANCASIteration();


	virtual int getRootsInInterval(ANCASCubicPolynomial P, double result[3]);

	//variables
	TcaCalculation::sPointData m_dataPoints[4];
	double m_Tau[4];
	double m_fd[4], m_fx[4], m_fy[4], m_fz[4];
	ANCASCubicPolynomial m_CfdotTau;
	ANCASCubicPolynomial m_Qx, m_Qy, m_Qz;
	double m_roots[3];

};



#endif //SHIELD_ANCAS_H