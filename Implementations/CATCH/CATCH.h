#ifndef SHIELD_CATCH_H    // Check if the symbol SHIELD_CATCH_H is not defined
#define SHIELD_CATCH_H    // Define the symbol SHIELD_CATCH_H

#include <limits>
#include <math.h>
#include "ITcaAlgorithm.h"
#include "TCA_Calculation.h"
#include "IRootsFindAlg.h"


// Conjunction Assessment Through Chebyshev Polynomials (CATCH)method
using namespace TCA_Calculation;

class CPP
{
public:
	CPP(int degree);
	~CPP();
	void fitCPP(double intervalStart, double intervalEnd, double* g);
	double getValue(double x);
	double *coefficients;

private:
	const double pi = 2 * acos(0.0);
	int m_degree;
	double getX(double a, double b, int j);
	void calculateInterpolationMatrix();
	int getPj(int j);
	double getTj(double x, int j);
	double **interpolationMatrix;
	double m_intervalStart, m_intervalEnd;
};

/// <summary>
/// Implementation of the CATCH(Conjunction Assessment Through Chebyshev Polynomials) algorithm bases on the article Satellite closest approach calculation through Chebyshev Proxy Polynomials by Elad Denenberg
/// </summary>
class CATCH : public ITcaAlgorithm
{
public:
	CATCH(IRootsFindAlg* rootsFinder, int degree);
	~CATCH();
	TCA runAlgorithm(sPointData * pointsInTime, double* timePoints, int lastPointIndex);
protected:
	IRootsFindAlg* m_rootsFinder;
private:
	//member variables
	int m_degree;
	double *m_fd, *m_fx, *m_fy, *m_fz;
	double *Tau;

	CPP *m_FdCpp, *m_xCpp, *m_yCpp, *m_zCpp;


};




#endif //SHIELD_CATCH_H