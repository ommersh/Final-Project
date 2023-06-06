#pragma once
#include "TCA_Calculation.h"
#include "Functions.h"
#include <limits>
#include <math.h>
#include "Functions.h"
#include <Eigen/Dense>

using namespace TCA_Calculation;
using namespace Eigen;


class CubicPolynomial
{
public:
	Vector4d coefficients;
	double getValue(double x)
	{
		return coefficients(0) + coefficients(1) * x + coefficients(2) * x * x + coefficients(3) * x * x * x;
	}
	void createCoefficients(double f[4], double Tau[4]);
};

class ANCAS
{
public:
	TCA ANCASAlgorithm(Vector3d Object1Location[4], Vector3d Object2Location[4], Vector3d Object1Velocity[4], Vector3d Object2Velocity[4],
		double timePoints[4]);
private:
	Vector3d calculateAccelelation(Vector3d ObjectLocation);
	Vector3d findCubicPolynomialRoots(CubicPolynomial c);
	void calculateQuinticPolynomial(QuinticPolynomial& q, double f_tn, double f_dot_tn, double f_ddot_tn, double f_tn_1, double f_dott_n1, double f_ddott_n1, double delta_t);

	const double earthRadius = 6378.1363;//km
	const double ePower2 = earthRadius * earthRadius;
	const double J2 = 1.08262668e-3;
	const double Mu = 398600.442; //km^3/s^2
};



