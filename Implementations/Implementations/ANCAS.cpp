#include "ANCAS.h"


void CubicPolynomial::createCoefficients(Function<double>* f, double Tau[4], int offset)
{
	double T1_1, T1_2,T1_3, T2_1, T2_2, T2_3;
	T1_1 = Tau[1];
	T1_2 = pow(Tau[1], 2);
	T1_3 = pow(Tau[1], 3);
	T2_1 = Tau[2];
	T2_2 = pow(Tau[2], 2);
	T2_3 = pow(Tau[2], 3);
	double Lambda = T1_3* T2_2 + T1_2* T2_1 + T1_1* T2_3 - T1_3* T2_1 - T1_1* T2_2;
	coefficients(0) = f->getValue(offset + 0);
	coefficients(1) = ((T2_3 - T2_2) * (f->getValue(offset + 1) - f->getValue(offset + 0)) + (T1_2 - T1_3) * (f->getValue(offset + 2) - f->getValue(offset + 0))
		+ (T1_3 * T2_2 - T1_2 * T2_3) * (f->getValue(offset + 3) - f->getValue(offset + 0))) / Lambda;

	coefficients(2) = ((T2_1 - T2_3) * (f->getValue(offset + 1) - f->getValue(offset + 0)) + (T1_1 - T1_2) * (f->getValue(offset + 2) - f->getValue(offset + 0))
		+ (T1_1 * T2_3 - T1_3 * T2_1) * (f->getValue(offset + 3) - f->getValue(offset + 0))) / Lambda;

	coefficients(3) = ((T2_2 - T2_1) * (f->getValue(offset + 1) - f->getValue(offset + 0)) + (T1_1 - T1_2) * (f->getValue(offset + 2) - f->getValue(offset + 0))
		+ (T1_2 * T2_1 - T1_1 * T2_2) * (f->getValue(offset + 3) - f->getValue(offset + 0))) / Lambda;
}





TCA ANCAS::ANCASAlgorithm(VectorFunction* locationInTimeObject1, VectorFunction* locationInTimeObject2, VectorFunction* velocityInTimeObject1, VectorFunction* velocityInTimeObject2,
	double* timePoints, int lastPointIndex)
{
	TCA tca;
	tca.time = 0;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	//1. Prepare the variables
	double Tau[4];
	RelativeDistanceFunction relativeVelocity(velocityInTimeObject1, velocityInTimeObject2);
	RelativeDistanceFunction relativeLocation(locationInTimeObject1, locationInTimeObject2);
	Fd fd = Fd(&relativeLocation, &relativeVelocity);
	RelativeFunctionInIndex Fx(locationInTimeObject1, locationInTimeObject2, 0);
	RelativeFunctionInIndex Fy(locationInTimeObject1, locationInTimeObject2, 1);
	RelativeFunctionInIndex Fz(locationInTimeObject1, locationInTimeObject2, 2);	
	CubicPolynomial C_fdot_tau;
	CubicPolynomial Qx, Qy, Qz;
	Vector3d roots;
	int startPointIndex, endPointIndex;
	int roundNumber = 0;
	int offset,numberOfRoots;
	double tau, tempDistance;
	startPointIndex = 0;
	endPointIndex = 3;
	//run over all the data
	while (endPointIndex <= lastPointIndex)
	{
		offset = (3)*roundNumber;
		for (int i = 0; i < 4; i++)
		{
			//Tau = [0,Tau2,Tau3,1]
			Tau[i] = (timePoints[offset + i] - timePoints[offset + 0]) / (timePoints[offset + 3] - timePoints[offset + 0]);
		}
		//2.Calculate the cubic polynomial Ctau
		C_fdot_tau.createCoefficients(&fd, Tau, offset);

		//3.Find the real roots
		numberOfRoots = findCubicPolynomialRoots(C_fdot_tau, roots);
		//4.Find the minimum distance
		for (int i = 0; i < numberOfRoots; i++)
		{
			if (i == 0)
			{
				tau = 0;
				tempDistance = 0;
				//5. Calculate polynomial for x,y,z
				Qx.createCoefficients(&Fx, Tau, offset);
				Qy.createCoefficients(&Fy, Tau, offset);
				Qz.createCoefficients(&Fz, Tau, offset);
			}
			tau = roots(i);
			//Eq.7
			tempDistance = sqrt(pow(Qx.getValue(tau), 2) + pow(Qy.getValue(tau), 2) + pow(Qz.getValue(tau), 2));
			if (tempDistance < tca.distance)
			{
				tca.distance = tempDistance;
				//Eq.8
				tca.time = timePoints[offset + 0] + tau * (timePoints[offset + 3] - timePoints[offset + 0]);
			}
		}

		//
		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + 3;
		roundNumber++;
	}
	return tca;
}

//#include "../../boost_1_82_0/boost/math/tools/quadratic_roots.hpp"

int ANCAS::findCubicPolynomialRoots(CubicPolynomial P, Vector3d &result)
{
	//calculate roots
	double a = P.coefficients(3);
	double b = P.coefficients(2);
	double c = P.coefficients(1);
	double d = P.coefficients(0);
	double temp;
	//the cubic equasion is ax^3 + bx^2 + cx + d = 0
	//int num_roots = boost::math::tools::quadratic_roots(coefficients[0], coefficients[1], coefficients[2], coefficients[3], roots[0], roots[1], roots[2]);
	double roots[3];
	int numberOfRoots = 0,numberOfRootsInRange = 0;
	calculateCubicRoots(a, b, c, d, roots, numberOfRoots);
	for (int i = 0; i < numberOfRoots; i++)
	{
		if (roots[i] >= 0 && roots[i] < 1)
		{
			result(numberOfRootsInRange) = roots[i];
			numberOfRootsInRange++;
		}
	}
	return numberOfRootsInRange;
}

#include <cmath>
// Function to calculate the roots of a cubic equation using Cardano's method
void ANCAS::calculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots) {
	if (a == 0.0) {
		std::cout << "Not a cubic equation." << std::endl;
		return;
	}
	double p = (3 * a * c - b * b) / (3 * a * a);
	double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);
	double discriminant = q * q / 4 + p * p * p / 27;
	if (discriminant > 0) {
		double r = -q / 2 + std::sqrt(discriminant);
		double s = std::cbrt(r);
		double t = -q / 2 - std::sqrt(discriminant);
		double u = std::cbrt(t);

		double root1 = s + u - b / (3 * a);
		numberOfRoots = 1;
		roots[0] = root1;
	}
	else if (discriminant == 0) {
		double r = -q / 2;
		double s = std::cbrt(r);

		double root1 = 2 * s - b / (3 * a);
		double root2 = -s - b / (3 * a);
		numberOfRoots = 2;
		roots[0] = root1;
		roots[1] = root2;
	}
	else {
		double alpha = std::sqrt(-p / 3);
		double beta = std::acos(-q / (2 * std::sqrt(-p * p * p / 27)));
		double root1 = 2 * alpha * std::cos(beta / 3) - b / (3 * a);
		double root2 = 2 * alpha * std::cos((beta + 2 * M_PI) / 3) - b / (3 * a);
		double root3 = 2 * alpha * std::cos((beta + 4 * M_PI) / 3) - b / (3 * a);

		numberOfRoots = 3;
		roots[0] = root1;
		roots[1] = root2;
		roots[2] = root3;
	}
}