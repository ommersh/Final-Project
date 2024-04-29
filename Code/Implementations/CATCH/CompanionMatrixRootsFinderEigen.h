#ifndef SHIELD_CompanionMatrixRootsFinderEigen_H    // Check if the symbol SHIELD_CompanionMatrixRootsFinderEigen_H is not defined
#define SHIELD_CompanionMatrixRootsFinderEigen_H    // Define the symbol SHIELD_CompanionMatrixRootsFinderEigen_H

#include <Eigen/Eigenvalues>

#include "CompanionMatrixRootsFinder.h"
#include <limits>
#include <math.h>





using namespace Eigen;
/// <summary>
/// Implementation of the CompanionMatrixRootsFinder class using the Eigen library
/// </summary>
class CompanionMatrixRootsFinderEigen : public CompanionMatrixRootsFinder
{
public:
	void Init(int degree) override;
	int findEigenValues() override;
private:
	Eigen::MatrixXd m_EigenCompanionMatrix;
	Eigen::EigenSolver<Eigen::MatrixXd> solver;
};

#endif //SHIELD_CompanionMatrixRootsFinderEigen_H
