#include "CompanionMatrixRootsFinderArmadillo.h"





void CompanionMatrixRootsFinderArmadillo::init(int degree)
{
	CompanionMatrixRootsFinder::init(degree);
    m_armaCompanionMatrix = arma::mat(m_polynomialDegree, m_polynomialDegree);

}



int CompanionMatrixRootsFinderArmadillo::findEigenValues()
{
    static const double eigenImagError = 0.0001;

    int numberOfRealRoots = 0;
    // Initialize a matrix of the given size with zeros
    for (int i = 0; i < m_polynomialDegree; i++)
    {
        for (int j = 0; j < m_polynomialDegree; j++)
        {
            m_armaCompanionMatrix(i, j) = m_companionMatrix[i][j];
        }
    }


    arma::cx_vec eigval = arma::eig_gen(m_armaCompanionMatrix);



    for (int i = 0; i < eigval.size(); ++i) {
        if (fabs(eigenImagError - eigval(i).imag()) <= eigenImagError && eigval(i).real() <= 1 && eigval(i).real() >= -1)
        {
            m_eigenValues[numberOfRealRoots++] = eigval(i).real();
        }
    }
	return numberOfRealRoots;
}