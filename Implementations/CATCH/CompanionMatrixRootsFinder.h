#ifndef SHIELD_CompanionMatrixRootsFinder_H    // Check if the symbol SHIELD_CompanionMatrixRootsFinder_H is not defined
#define SHIELD_CompanionMatrixRootsFinder_H    // Define the symbol SHIELD_CompanionMatrixRootsFinder_H

#include <Eigen/Eigenvalues>

#include "IRootsFindAlg.h"
#include <limits>
#include <math.h>





using namespace Eigen;

class CompanionMatrixRootsFinder : public IRootsFindAlg
{
public:
	CompanionMatrixRootsFinder(int degree);
	int findRoots(double* coefficients, int degree, double* roots);
	VectorXd getRoots();
private:
	void InitCompanionMatrix();
	void computeCompanionMatrix();
	int getPj(int j);
	int Delta(int q, int r);
	int m_polynomialDegree;
	VectorXd m_coefficients;
	MatrixXd m_companionMatrix;
};

#endif //SHIELD_CompanionMatrixRootsFinder_H
