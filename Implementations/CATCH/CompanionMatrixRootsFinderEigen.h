#ifndef SHIELD_CompanionMatrixRootsFinderEigen_H    // Check if the symbol SHIELD_CompanionMatrixRootsFinderEigen_H is not defined
#define SHIELD_CompanionMatrixRootsFinderEigen_H    // Define the symbol SHIELD_CompanionMatrixRootsFinderEigen_H

#include <Eigen/Eigenvalues>

#include "CompanionMatrixRootsFinder.h"
#include <limits>
#include <math.h>





using namespace Eigen;

class CompanionMatrixRootsFinderEigen : public CompanionMatrixRootsFinder
{
public:
	void init(int degree) override;
	int findEigenValues() override;
private:
	Eigen::MatrixXd m_EigenCompanionMatrix;
};

#endif //SHIELD_CompanionMatrixRootsFinderEigen_H
