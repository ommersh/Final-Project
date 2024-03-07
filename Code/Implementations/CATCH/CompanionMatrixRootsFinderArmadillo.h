#ifndef SHIELD_CompanionMatrixRootsFinderArmadillo_H    // Check if the symbol SHIELD_CompanionMatrixRootsFinderArmadillo_H is not defined
#define SHIELD_CompanionMatrixRootsFinderArmadillo_H    // Define the symbol SHIELD_CompanionMatrixRootsFinderArmadillo_H


#include "CompanionMatrixRootsFinder.h"
#include <limits>
#include <math.h>
#include <armadillo>


class CompanionMatrixRootsFinderArmadillo : public CompanionMatrixRootsFinder
{
public:
	void init(int degree) override;
	int findEigenValues() override;
private:	
	arma::mat m_armaCompanionMatrix;

};

#endif //SHIELD_CompanionMatrixRootsFinderArmadillo_H
