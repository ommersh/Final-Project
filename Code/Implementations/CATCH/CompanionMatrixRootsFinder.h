#ifndef SHIELD_CompanionMatrixRootsFinder_H    // Check if the symbol SHIELD_CompanionMatrixRootsFinder_H is not defined
#define SHIELD_CompanionMatrixRootsFinder_H    // Define the symbol SHIELD_CompanionMatrixRootsFinder_H

#include "IRootsFindAlg.h"
#include "TcaCalculation.h"
#include <limits>
#include <math.h>

/// <summary>
/// Abstact class implementing the companion matrix roots finding algorithm
/// To use you need to impelemts the eigen values finding function 
/// </summary>
class CompanionMatrixRootsFinder : public IRootsFindAlg
{
public:
	CompanionMatrixRootsFinder();
	virtual void init(int degree) override;
	int findRoots(double* coefficients, int degree, double* roots) override;
private:
	void InitCompanionMatrix();
	void computeCompanionMatrix();
	int getPj(int j);
	int Delta(int q, int r);
protected:
	int m_polynomialDegree;
	double m_coefficients[CATCH_MAX_DEGREE+1];
	double m_companionMatrix[CATCH_MAX_DEGREE][CATCH_MAX_DEGREE];
	double m_eigenValues[CATCH_MAX_DEGREE + 1];
	virtual int findEigenValues() = 0;

};

#endif //SHIELD_CompanionMatrixRootsFinder_H
