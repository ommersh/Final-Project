#include "ANCAS.h"


void CubicPolynomial::createCoefficients(double f[4], double Tau[4])
{
	double T1_1, T1_2,T1_3, T2_1, T2_2, T2_3;
	T1_1 = Tau[1];
	T1_2 = pow(Tau[1], 2);
	T1_3 = pow(Tau[1], 3);
	T2_1 = Tau[2];
	T2_2 = pow(Tau[2], 2);
	T2_3 = pow(Tau[2], 3);
	double Lambda = T1_3* T2_2 + T1_2* T2_1 + T1_1* T2_3 - T1_3* T2_1 - T1_1* T2_2;
	coefficients(0) = f[0];
	coefficients(1) = (T2_3 - T2_2) * (f[1] - f[0]) + (T1_2 - T1_3) * (f[2] - f[0])
		+ (T1_3 * T2_2 - T1_2 * T2_3) * (f[3] - f[0]) / Lambda;

	coefficients(2) = (T2_1 - T2_3) * (f[1] - f[0]) + (T1_1 - T1_2) * (f[2] - f[0])
		+ (T1_1 * T2_3 - T1_3 * T2_1) * (f[3] - f[0]) / Lambda;

	coefficients(3) = (T2_2 - T2_1) * (f[1] - f[0]) + (T1_1 - T1_2) * (f[2] - f[0])
		+ (T1_2 * T2_1 - T1_1 * T2_2) * (f[3] - f[0]) / Lambda;
}





TCA ANCAS::ANCASAlgorithm(Vector3d Object1Location[4], Vector3d Object2Location[4], Vector3d Object1Velocity[4], Vector3d Object2Velocity[4],
	double timePoints[4])
{
	TCA tca;
	tca.time = 0;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	//1. Prepare the variables
	double Tau[4];
	double Fd[4], Fx[4], Fy[4], Fz[4];
	Vector3d F[4];
	for (int i = 0; i < 4; i++)
	{
		//Tau = [0,Tau2,Tau3,1]
		Tau[i] = (timePoints[i] - timePoints[0]) / (timePoints[3] - timePoints[0]);
		//Fd = 2*rddot * rd
		// 2 * (v1 - v2)*(r1 - r2)...
		Fd[i] = 2 * ((Object1Velocity[i] - Object2Velocity[i]).dot(Object1Location[i] - Object2Location[i]));
		F[i] = Object1Location[i] - Object2Location[i];
		Fx[i] = F[i](0);
		Fy[i] = F[i](0);
		Fz[i] = F[i](0);

	}
	CubicPolynomial C_fdot_tau;
	CubicPolynomial Qx, Qy, Qz;

	//2.Calculate the cubic polynomial Ctau
	C_fdot_tau.createCoefficients(Fd, Tau);

	//3. Calculate polynomial for x,y,z
	Qx.createCoefficients(Fx, Tau);
	Qy.createCoefficients(Fy, Tau);
	Qz.createCoefficients(Fz, Tau);
	//3.Find the real roots
	Vector3d roots = findCubicPolynomialRoots(C_fdot_tau);
	//4.Find the minimum distance
	double tau = 0;
	double tempDistance = 0;
	for (int i = 0; i < roots.size(); i++)
	{
		tau = roots(i);
		//Eq.7
		tempDistance = sqrt(pow(Qx.getValue(tau), 2) + pow(Qy.getValue(tau), 2) + pow(Qz.getValue(tau), 2));
		if (tempDistance < tca.distance)
		{
			tca.distance = tempDistance;
			//Eq.8
			tca.time = timePoints[0] + tau * (timePoints[3] - timePoints[0]);
		}
	}
	return tca;
}


Vector3d ANCAS::findCubicPolynomialRoots(CubicPolynomial P)
{
	Vector3cd roots;
	//calculate roots
	double a = P.coefficients(3);
	double b = P.coefficients(2);
	double c = P.coefficients(1);
	double d = P.coefficients(0);

	std::complex<double> p, q, r; // Roots of the cubic equation
	// Calculate discriminant and intermediate values
	double discriminant = 18 * a * b * c * d - 4 * pow(b, 3) * d + pow(b, 2) * pow(c, 2) - 4 * a * pow(c, 3) - 27 * pow(a, 2) * pow(d, 2);
	double delta0 = pow(b, 2) - 3 * a * c;
	double delta1 = 2 * pow(b, 3) - 9 * a * b * c + 27 * pow(a, 2) * d;

	if (discriminant == 0 && delta0 == 0 && delta1 == 0) {
		// Case of three equal real roots
		p = q = r = -b / (3 * a);
	}
	else if (discriminant > 0) {
		// Case of one real root and two complex conjugate roots
		double sqrt_discriminant = std::sqrt(discriminant);
		double C = pow((delta1 + sqrt_discriminant) / 2, 1.0 / 3.0);
		double D = pow((delta1 - sqrt_discriminant) / 2, 1.0 / 3.0);

		p = (-b / (3 * a)) + C + D;
		q = std::complex<double>(-1.0 / (3 * a), sqrt(3.0) / (3 * a)) * (C + D) - 0.5 * (C - D) * std::complex<double>(0, 1);
		r = std::complex<double>(-1.0 / (3 * a), -sqrt(3.0) / (3 * a)) * (C + D) - 0.5 * (C - D) * std::complex<double>(0, 1);
	}
	else {
		// Case of three distinct real roots
		double C = std::sqrt(-delta0 / 3);
		double theta = std::acos(3 * a * C / b) / 3;

		p = (-b / (3 * a)) - 2 * C * std::cos(theta);
		q = (-b / (3 * a)) + C * (std::cos(theta) + std::sqrt(3.0) * std::sin(theta));
		r = (-b / (3 * a)) + C * (std::cos(theta) - std::sqrt(3.0) * std::sin(theta));
	}
	roots(0) = p;
	roots(1) = q;
	roots(2) = r;
	return roots.real();
}

