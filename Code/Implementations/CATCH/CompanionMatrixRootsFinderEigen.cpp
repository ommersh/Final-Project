#include "CompanionMatrixRootsFinderEigen.h"


void CompanionMatrixRootsFinderEigen::Init(int degree)
{
	CompanionMatrixRootsFinder::Init(degree);
	m_EigenCompanionMatrix = Eigen::MatrixXd(degree, degree);
}



int CompanionMatrixRootsFinderEigen::findEigenValues()
{
	static const double eigenImagError = 0.0001;
	for (int i = 0; i < m_polynomialDegree; i++)
	{
		for (int j = 0; j < m_polynomialDegree; j++)
		{
			m_EigenCompanionMatrix(i, j) = m_companionMatrix[i][j];
		}
	}
	//vector<double> eigenvalues = ; // Compute the eigenvalues
	EigenSolver<Eigen::MatrixXd> solver(m_EigenCompanionMatrix);
	VectorXcd eigenvalues = solver.eigenvalues();
	int numberOfRealRoots = 0;
	for (int i = 0; i < eigenvalues.size(); i++)
	{
		if (fabs(eigenImagError - eigenvalues(i).imag()) <= eigenImagError && eigenvalues(i).real() <= 1 && eigenvalues(i).real() >= -1)
		{
			m_eigenValues[numberOfRealRoots++] = eigenvalues(i).real();
		}
	}
	return numberOfRealRoots;
}
