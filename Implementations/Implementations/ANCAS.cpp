#include "ANCAS.h"

TCA ANCAS::ANCASAlgorithm(Vector3d Object1Location_tn, Vector3d Object1Location_tn1, Vector3d Object2Location_tn,
	Vector3d Object2Location_tn1, Vector3d Object1Velocity_tn, Vector3d Object1Velocity_tn1, Vector3d Object2Velocity_tn,
	Vector3d Object2Velocity_tn1, double tn, double tn1)
{
	TCA tca;
	tca.time = 0;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	//1. Prepare the variables
	double delta_t = tn1 - tn;
	Vector3d v_relativeDistance_tn = Object1Location_tn - Object2Location_tn;
	Vector3d v_relativeDistance_tn1 = Object1Location_tn1 - Object2Location_tn1;
	Vector3d v_relativeVelocity_tn = Object1Velocity_tn - Object2Velocity_tn;
	Vector3d v_relativeVelocity_tn1 = Object1Velocity_tn1 - Object2Velocity_tn1;
	Vector3d v_relativeAcceleration_tn = calculateAccelelation(Object1Location_tn) - calculateAccelelation(Object2Location_tn);
	Vector3d v_relativeAcceleration_tn1 = calculateAccelelation(Object1Location_tn1) - calculateAccelelation(Object2Location_tn1);
	//Eq.5(a)
	double f_tn = v_relativeDistance_tn.dot(v_relativeDistance_tn);
	double f_tn1 = v_relativeDistance_tn1.dot(v_relativeDistance_tn1);
	//Eq.5(b)
	double fdot_tn = 2 * v_relativeVelocity_tn.dot(v_relativeDistance_tn);
	double fdot_tn1 = 2 * v_relativeVelocity_tn1.dot(v_relativeDistance_tn1);
	//Eq.5(c)
	double fddot_tn = 2 * (v_relativeAcceleration_tn.dot(v_relativeDistance_tn) + v_relativeVelocity_tn.dot(v_relativeVelocity_tn));
	double fddot_tn1 = 2 * (v_relativeAcceleration_tn1.dot(v_relativeDistance_tn1) + v_relativeVelocity_tn1.dot(v_relativeVelocity_tn1));
	//2.Calculate the cubic polynomial Ctau
	CubicPolynomial C_fdot_tau;
	//Eq.6(a)
	C_fdot_tau.coefficients(0) = fdot_tn;
	//Eq.6(b)
	C_fdot_tau.coefficients(1) = fddot_tn* delta_t;
	//Eq.6(c)
	C_fdot_tau.coefficients(2) = -3* fdot_tn - 2* fddot_tn* delta_t + 3* fdot_tn1 - fddot_tn1* delta_t;
	//Eq.6(d)
	C_fdot_tau.coefficients(3) = 2 * fdot_tn + fddot_tn * delta_t - 2 * fdot_tn1 + fddot_tn1 * delta_t;
	//3. Calculate the quintic polynomial
	QuinticPolynomial Qi, Qj, Qk;
	//Qi
	calculateQuinticPolynomial(Qi, v_relativeDistance_tn(0), v_relativeVelocity_tn(0), v_relativeAcceleration_tn(0),
		v_relativeDistance_tn1(0), v_relativeVelocity_tn1(0), v_relativeAcceleration_tn1(0), delta_t);
	//Qj
	calculateQuinticPolynomial(Qj, v_relativeDistance_tn(1), v_relativeVelocity_tn(1), v_relativeAcceleration_tn(1),
		v_relativeDistance_tn1(1), v_relativeVelocity_tn1(1), v_relativeAcceleration_tn1(1), delta_t);
	//Qk
	calculateQuinticPolynomial(Qk, v_relativeDistance_tn(2), v_relativeVelocity_tn(2), v_relativeAcceleration_tn(2),
		v_relativeDistance_tn1(2), v_relativeVelocity_tn1(2), v_relativeAcceleration_tn1(2), delta_t);
	//3.Find the real roots
	Vector3d roots = findCubicPolynomialRoots(C_fdot_tau);
	//4.Find the minimum distance
	double tau = 0;
	double tempDistance = 0;
	for (int i = 0; i < roots.size(); i++)
	{
		tau = roots(i);
		//Eq.7
		tempDistance = sqrt(pow(Qi.getValue(tau), 2) + pow(Qj.getValue(tau), 2) + pow(Qk.getValue(tau), 2));
		if (tempDistance < tca.distance)
		{
			tca.distance = tempDistance;
			//Eq.8
			tca.time = tn + tau * delta_t;
		}
	}
	return tca;
}

void ANCAS::calculateQuinticPolynomial(QuinticPolynomial &q,double f_tn, double f_dot_tn, double f_ddot_tn, double f_tn_1,
	double f_dott_n1, double f_ddott_n1, double delta_t)
{
	//Eq.2(a)
	q.coefficients(0) = f_tn;
	//Eq.2(b)
	q.coefficients(1) = f_dot_tn*delta_t;
	//Eq.2(c)
	q.coefficients(2) = 0.5 * f_ddot_tn * pow(delta_t,2);
	//Eq.2(d)
	q.coefficients(3) = -10 * f_tn - 6 * f_dot_tn * delta_t - 1.5 * f_ddot_tn * pow(delta_t, 2) +
		10 * f_tn_1 - 4 * f_dott_n1 * delta_t + 0.5 * f_ddott_n1 * pow(delta_t, 2);
	//Eq.2(e)
	q.coefficients(4) = 15 * f_tn + 8 * f_dot_tn * delta_t + 1.5 * f_ddot_tn * pow(delta_t, 2) 
		- 15 * f_tn_1 + 7 * f_dott_n1 * delta_t - f_ddott_n1 * pow(delta_t, 2);
	//Eq.2(f)
	q.coefficients(5) = - 6 * f_tn - 3 * f_dot_tn * delta_t - 0.5 * f_ddot_tn * pow(delta_t, 2) 
		+ 6 * f_tn_1 - 3 * f_dott_n1 * delta_t + 0.5 * f_ddott_n1 * pow(delta_t, 2);

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


Vector3d ANCAS::calculateAccelelation(Vector3d r)
{
	Vector3d Accelelation;
	double R_Norm = r.norm();
	double rk_Power_2 = pow(r(2), 2);
	//vector with IJK units 
	//Eq.4(a)
	Accelelation(0) = -Mu * r(0) / pow(R_Norm, 3) * (1 + 3 * J2 * ePower2 / (2 * pow(R_Norm, 2)) * (1 - 5 * rk_Power_2 / pow(R_Norm, 2)));
	//Eq.4(b)
	Accelelation(1) = -Mu * r(1) / pow(R_Norm, 3) * (1 + 3 * J2 * ePower2 / (2 * pow(R_Norm, 2)) * (1 - 5 * rk_Power_2 / pow(R_Norm, 2)));
	//Eq.4(c)
	Accelelation(2) = -Mu * r(2) / pow(R_Norm, 3) * (1 + 3 * J2 * ePower2 / (2 * pow(R_Norm, 2)) * (3 - 5 * rk_Power_2 / pow(R_Norm, 2)));
}
