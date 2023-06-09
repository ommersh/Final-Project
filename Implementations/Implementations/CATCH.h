#pragma once
#include "TCA_Calculation.h"
#include "Functions.h"
#include <limits>
#include <math.h>
#include <Eigen/Eigenvalues>
#include <iostream>

using namespace Eigen;

#define _USE_MATH_DEFINES

#define N 15
const double pi = 2 * acos(0.0);
// Conjunction Assessment Through Chebyshev Polynomials (CATCH)method
using namespace TCA_Calculation;
struct DistanceInTime {
	int time;
	double distance;
};
class CATCH
{
public:
	CATCH() {};
	TCA CatchAlgorithm(VectorFunction* locationInTimeObject1, VectorFunction* locationInTimeObject2,
		VectorFunction* velocityInTimeObject1, VectorFunction* velocityInTimeObject2, double* timePoints, int lastPointIndex);
private:
	//member variables
	int TauSize = N;

};

class CPP
{
public:
	CPP() : coefficients(N+1), interpolationMatrix(N+1, N+1), companionMatrix(N, N) { ; }
	void fitCPP(double intervalStart, double intervalEnd, Function<double>* g, int offset);
	VectorXd getRoots();
	double getValue(double x);
private:
	double getX(double a, double b, int j);
	void computeCompanionMatrix();
	void calculateInterpolationMatrix();
	int getPj(int j);
	int Delta(int q, int r);
	double getTj(double x, int j);
	VectorXd coefficients;
	MatrixXd interpolationMatrix;
	MatrixXd companionMatrix;
	double m_intervalStart, m_intervalEnd;
};


