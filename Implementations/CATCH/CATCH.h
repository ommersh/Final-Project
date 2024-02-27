#ifndef SHIELD_CATCH_H    // Check if the symbol SHIELD_CATCH_H is not defined
#define SHIELD_CATCH_H    // Define the symbol SHIELD_CATCH_H

#include <limits>
#include <math.h>
#include "ITcaAlgorithm.h"
#include "TCA_Calculation.h"
#include "IRootsFindAlg.h"


// Conjunction Assessment Through Chebyshev Polynomials (CATCH)method
using namespace TcaCalculation;


class CPP
{
public:
	CPP();
	~CPP();
	void init( int degree);

	void fitCPP(double intervalStart, double intervalEnd, double g[CATCH_MAX_DEGREE + 1]);
	double getValue(double x);
	double coefficients[CATCH_MAX_DEGREE + 1];

protected:
	double m_interpolationMatrix[CATCH_MAX_DEGREE + 1][CATCH_MAX_DEGREE + 1];

private:
	const double pi = 2 * acos(0.0);
	int m_degree;
	double getX(double a, double b, int j);
	void calculateInterpolationMatrix();
	int getPj(int j);
	double getTj(double x, int j);
	double m_intervalStart, m_intervalEnd;
};

/// <summary>
/// Implementation of the CATCH(Conjunction Assessment Through Chebyshev Polynomials) algorithm bases on the article Satellite closest approach calculation through Chebyshev Proxy Polynomials by Elad Denenberg
/// </summary>
class CATCH : public ITcaAlgorithm
{
public:
	CATCH();
	~CATCH();
	void init(IRootsFindAlg* rootsFinder, int degree);

	TCA runAlgorithm(TcaCalculation::sPointData * pointsInTime, double* timePoints, int lastPointIndex);
protected:
	IRootsFindAlg* m_rootsFinder;
private:
	//member variables
	int m_degree;
	double m_fd[CATCH_MAX_DEGREE + 1];
	double m_fx[CATCH_MAX_DEGREE + 1];
	double m_fy[CATCH_MAX_DEGREE + 1];
	double m_fz[CATCH_MAX_DEGREE + 1];
	double Tau[CATCH_MAX_DEGREE];

	CPP m_FdCpp;
	CPP m_xCpp;
	CPP m_yCpp;
	CPP m_zCpp;


};




#endif //SHIELD_CATCH_H