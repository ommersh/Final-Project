#pragma once
#include "TCA_Calculation.h"
#include "Functions.h"
#include <limits>
#include <math.h>
#include "Functions.h"
#include <Eigen/Dense>
#include <cmath>
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif
using namespace TCA_Calculation;
using namespace Eigen;


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

class ANCAS
{
public:
	TCA ANCASAlgorithm(sPointData* pointsInTime, double* timePoints, int lastPointIndex);
private:
	int findCubicPolynomialRoots(CubicPolynomial P, Vector3d &result);
	void calculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots);
};



