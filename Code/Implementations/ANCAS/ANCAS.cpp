#include "ANCAS.h"

/// <summary>
/// Create the cubic polynomial coefficients for ANCAS, based on 4 data points
/// Eq.1(f-j)
/// </summary>
/// <param name="f">Array with 4 points in time</param>
/// <param name="Tau"></param>
bool ANCASCubicPolynomial::createCoefficients(double f[4], double Tau[4])
{
	bool coefficientsCreated = false;
	double T1_1, T1_2,T1_3, T2_1, T2_2, T2_3;
	T1_1 = Tau[1];
	T1_2 = pow(Tau[1], 2);
	T1_3 = pow(Tau[1], 3);
	T2_1 = Tau[2];
	T2_2 = pow(Tau[2], 2);
	T2_3 = pow(Tau[2], 3);
	//Eq.1(j)
	double Lambda = T1_3 * T2_2;
	Lambda += T1_2 * T2_1;
	Lambda += T1_1 * T2_3;
	Lambda -= T1_1 * T2_2;
	Lambda -= T1_3 * T2_1;
	Lambda -= T1_2 * T2_3;

	//check for zero
	if (0 != Lambda)
	{
		//Eq.1(f) 
		coefficients[0] = f[0];
		//Eq.1(g)
		coefficients[1] = ((T2_3 - T2_2) * (f[1] - f[0]) + (T1_2 - T1_3) * (f[2] - f[0])
			+ (T1_3 * T2_2 - T1_2 * T2_3) * (f[3] - f[0])) / Lambda;
		//Eq.1(h)
		coefficients[2] = ((T2_1 - T2_3) * (f[1] - f[0]) + (T1_3 - T1_1) * (f[2] - f[0])
			+ (T1_1 * T2_3 - T1_3 * T2_1) * (f[3] - f[0])) / Lambda;
		//Eq.1(i)
		coefficients[3] = ((T2_2 - T2_1) * (f[1] - f[0]) + (T1_1 - T1_2) * (f[2] - f[0])
			+ (T1_2 * T2_1 - T1_1 * T2_2) * (f[3] - f[0])) / Lambda;
		coefficientsCreated = true;
	}
	return coefficientsCreated;
}

double ANCASCubicPolynomial::getValue(double x)
{
	return coefficients[0] + coefficients[1] * x + coefficients[2] * pow(x, 2) + coefficients[3] * pow(x, 3);
}


/// <summary>
/// The ANCAS algorithm
/// </summary>
/// <param name="pointsInTime">
/// Points in time(r1,r2,v1,v2)
/// </param>
/// <param name="timePoints">
/// The time points, respective times for the data points
/// </param>
/// <param name="lastPointIndex">
///	The index of the last point(data + time) 
/// </param>
/// <returns>
/// TCA, time of closest approach and the corresponding distance
/// </returns>
TCA ANCAS::RunAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex)
{
	TCA tca = { 0 };
	TCA tempTca;

	tca.time = 0;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	tca.numberOfPoints = 0;
	//1. Prepare the variables
	
	int startPointIndex, endPointIndex;
	int roundNumber = 0;
	int offset = 0;
	int numberOfRoots = 0;
	startPointIndex = 0;
	endPointIndex = 3;
	//run over all the data
	while (endPointIndex < lastPointIndex)
	{
		offset = (3)*roundNumber;
		for (int i = 0; i < 4; i++)
		{
			m_dataPoints[i] = pointsInTime[offset + i];
		}

		tempTca = ANCASIteration();
		if (tempTca.time == -1)
		{
			tca.numberOf_NoRootsFound++;
		}
		if (tempTca.distance < tca.distance)
		{
			tca.distance = tempTca.distance;
			tca.time = tempTca.time;
		}

		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + 3;
		roundNumber++;
	}


	tca.numberOfPoints = 1 + offset + 3;
	
	return tca;
}
TCA ANCAS::ANCASIteration()
{
	TCA tca;
	tca.time = -1;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	int numberOfRoots = 0;
	double tau, tempDistance;
	bool coefficientsCreated = true;
	for (int i = 0; i < 4; i++)
	{
		m_fx[i] = m_dataPoints[i].r1x - m_dataPoints[i].r2x;
		m_fy[i] = m_dataPoints[i].r1y - m_dataPoints[i].r2y;
		m_fz[i] = m_dataPoints[i].r1z - m_dataPoints[i].r2z;
		m_fd[i] = 2 * (((m_fx[i]) * (m_dataPoints[i].v1x - m_dataPoints[i].v2x)) +
			((m_fy[i]) * (m_dataPoints[i].v1y - m_dataPoints[i].v2y)) +
			((m_fz[i]) * (m_dataPoints[i].v1z - m_dataPoints[i].v2z)));
	}
	for (int i = 0; i < 4; i++)
	{
		//Tau = [0,Tau2,Tau3,1]
		m_Tau[i] = (m_dataPoints[i].time - m_dataPoints[0].time) / (m_dataPoints[3].time - m_dataPoints[0].time);
	}
	//2.Calculate the cubic polynomial Ctau
	coefficientsCreated = m_CfdotTau.createCoefficients(m_fd, m_Tau);

	//3. Calculate polynomial for x,y,z
	coefficientsCreated &= m_Qx.createCoefficients(m_fx, m_Tau);
	coefficientsCreated &= m_Qy.createCoefficients(m_fy, m_Tau);
	coefficientsCreated &= m_Qz.createCoefficients(m_fz, m_Tau);
	if (true == coefficientsCreated)
	{
		tau = 0;
		tempDistance = 0;
		//4.Find the real roots
		numberOfRoots = getRootsInInterval(m_CfdotTau, m_roots);
		//5.Find the minimum distance
		for (int i = 0; i < numberOfRoots; i++)
		{
			tau = m_roots[i];
			//Eq.7
			tempDistance = sqrt(pow(m_Qx.getValue(tau), 2) + pow(m_Qy.getValue(tau), 2) + pow(m_Qz.getValue(tau), 2));
			if (tempDistance < tca.distance)
			{
				tca.distance = tempDistance;
				//Eq.8
				tca.time = m_dataPoints[0].time + tau * (m_dataPoints[3].time - m_dataPoints[0].time);
			}
		}
	}
	return tca;
}
/// <summary>
/// Find the cubic polynomial roots in the interval [0 , 1)
/// </summary>
/// <param name="P">
/// The cubic polynomial
/// </param>
/// <param name="result">
/// vector for the result
/// </param>
/// <returns>
/// the number of roots found
/// </returns>
int ANCAS::getRootsInInterval(ANCASCubicPolynomial P, double result[3])
{
	//calculate roots
	double a = P.coefficients[3];
	double b = P.coefficients[2];
	double c = P.coefficients[1];
	double d = P.coefficients[0];
	//the cubic equasion is ax^3 + bx^2 + cx + d = 0
	double roots[3];
	int numberOfRoots = 0,numberOfRootsInInterval = 0;
	calculateCubicRoots(a, b, c, d, roots, numberOfRoots);
	for (int i = 0; i < numberOfRoots; i++)
	{
		if (roots[i] > 0 && roots[i] < 1)
		{
			result[numberOfRootsInInterval] = roots[i];
			numberOfRootsInInterval++;
		}
	}
	return numberOfRootsInInterval;
}

#include <cmath>
/// <summary>
/// Calculate all the real roots of a cubic equation ax^3 + bx^2 + cx + d
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <param name="d"></param>
/// <param name="roots"></param>
/// <param name="numberOfRoots"></param>
void ANCAS::calculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots) {
	//if a=0 its a second degree equation
	if (a == 0.0) {
		//if b = 0 and a = 0 its first degree equation
		if (b == 0)
		{
			//cx + d = 0
			// x = -d/c
			if (c != 0)
			{
				roots[0] = -d / c;
				numberOfRoots = 1;
				return;
			}
			else
			{
				//no roots
				numberOfRoots = 0;
				return;
			}
		}
		// bx^2 + cx + d
		//x12 = -c += sqrt ( c^2 - 4 bd)/2b
		double discriminant = c * c - 4 * b * d;
		if (discriminant < 0)
		{
			//no real roots
			numberOfRoots = 0;
			return;
		}
		else if (discriminant == 0)
		{
			//one real root
			numberOfRoots = 1;
			roots[0] = -c / (2 * b);
			return;
		}
		else
		{
			//two real roots
			discriminant = sqrt(discriminant);
			roots[0] = (-c + discriminant)/ (2 * b);
			roots[1] = (-c - discriminant) / (2 * b);
			numberOfRoots = 2;
			return;
		}
	}
	//Solve the cubic equation
	double p = (3 * a * c - b * b) / (3 * a * a);
	double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);
	double discriminant = q * q / 4 + p * p * p / 27;
	if (discriminant > 0) {
		double r = -q / 2 + std::sqrt(discriminant);
		double s = std::cbrt(r);
		double t = -q / 2 - std::sqrt(discriminant);
		double u = std::cbrt(t);
		roots[0] = s + u - b / (3 * a);
		numberOfRoots = 1;
	}
	else if (discriminant == 0) {
		double r = -q / 2;
		double s = std::cbrt(r);
		roots[0] = 2 * s - b / (3 * a);
		roots[1] = -s - b / (3 * a);
		numberOfRoots = 2;
	}
	else {
		double alpha = std::sqrt(-p / 3);
		double beta = std::acos(-q / (2 * std::sqrt(-p * p * p / 27)));
		roots[0] = 2 * alpha * std::cos(beta / 3) - b / (3 * a);
		roots[1] = 2 * alpha * std::cos((beta + 2 * M_PI) / 3) - b / (3 * a);
		roots[2] = 2 * alpha * std::cos((beta + 4 * M_PI) / 3) - b / (3 * a);
		numberOfRoots = 3;
	}
}