#pragma once
#include "TCA_Calculation.h"
#include "Functions.h"
#include <limits>
#include <math.h>
#include "Functions.h"
#include <Eigen/Dense>
#include <cmath>
#ifndef M_PI
const double M_PI = 2 * acos(0.0);
#endif
using namespace TCA_Calculation;
using namespace Eigen;

/// <summary>
/// Implementation of Cubic polynomial for ANCAS
/// </summary>
class CubicPolynomial
{
public:
	Vector4d coefficients;
	double getValue(double x)
	{
		return coefficients(0) + coefficients(1) * x + coefficients(2) * pow(x,2) + coefficients(3) * pow(x, 3);
	}
	void createCoefficients(double * f, double Tau[4]);
};
/// <summary>
/// Implentation of ANCAS(Alfano\Negron Close Approach Software) - based on Determining Satellite Close Approaches,Part 2 by Salvatore Alfano
/// 
/// </summary>
class ANCAS
{
public:
	TCA ANCASAlgorithm(sPointData* pointsInTime, double* timePoints, int lastPointIndex);
private:
	int findCubicPolynomialRoots(CubicPolynomial P, Vector3d &result);
	void calculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots);
};



