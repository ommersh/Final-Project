#pragma once
#include "TCA_Calculation.h"
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
	TCA CatchAlgorithm(sPointData * pointsInTime, double* timePoints, int lastPointIndex);
private:
	//member variables
	int TauSize = N;

};

class CPP
{
public:
	CPP();
	void fitCPP(double intervalStart, double intervalEnd, double * g);
	VectorXd getRoots();
	double getValue(double x);
private:
	double getX(double a, double b, int j);
	void InitCompanionMatrix();
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


