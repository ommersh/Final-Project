#pragma once
#include "TCA_Calculation.h"
#include <limits>
#include <math.h>
#define _USE_MATH_DEFINES

#define N 16

// Conjunction Assessment Through Chebyshev Polynomials (CATCH)method
using namespace TCA_Calculation;
struct DistanceInTime {
	int time;
	double distance;
};
class CATCH
{
public:
	CATCH(int order, double maxError);
	TCA CatchAlgorithm(definedFunctionInInterval g,int Gamma, int t_max);
private:
	void fitCPP(int intervalStart, int intervalEnd, definedFunctionInInterval g, double MaxError, int Order);
	void computeCompanionMatrix();
	void getEigenvalues();
	double calculateDistance();
	void calculateInterpolationMatrix(double interpolationMatrix[N + 1][N + 1]);
	double getX(double a, double b, int j);
	//member variables
	int Order;
	double MaxError;
	double Tau[1024];//temp size
	int TauSize;
	double CompanionMatrix[N][N];
	double pi = 2 * acos(0.0);
	double a[N + 1];

};



