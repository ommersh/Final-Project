#ifndef SHIELD_CATCH_H    // Check if the symbol SHIELD_CATCH_H is not defined
#define SHIELD_CATCH_H    // Define the symbol SHIELD_CATCH_H

#include <limits>
#include <math.h>
#include "ITcaAlgorithm.h"
#include "TCA_Calculation.h"
#include "IRootsFindAlg.h"


#define N 15
// Conjunction Assessment Through Chebyshev Polynomials (CATCH)method
using namespace TCA_Calculation;
/// <summary>
/// Implementation of the CATCH(Conjunction Assessment Through Chebyshev Polynomials) algorithm baes on Satellite closest approach calculation through Chebyshev Proxy Polynomials by Elad Denenberg
/// </summary>
class CATCH : public ITcaAlgorithm
{
public:
	CATCH(IRootsFindAlg* rootsFinder);
	TCA runAlgorithm(sPointData * pointsInTime, double* timePoints, int lastPointIndex);
protected:
	IRootsFindAlg* m_rootsFinder;
private:
	//member variables
	int TauSize = N;

};

class CPP
{
public:
	CPP();
	void fitCPP(double intervalStart, double intervalEnd, double * g);
	double getValue(double x);
	double coefficients[N + 1];

private:
	const double pi = 2 * acos(0.0);
	double getX(double a, double b, int j);
	void calculateInterpolationMatrix();
	int getPj(int j);
	double getTj(double x, int j);
	double interpolationMatrix[N + 1][N + 1];
	double m_intervalStart, m_intervalEnd;
};


#endif //SHIELD_CATCH_H