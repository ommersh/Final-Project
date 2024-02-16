#include "CompanionMatrixRootsFinder.h"



CompanionMatrixRootsFinder::CompanionMatrixRootsFinder(int degree) : m_coefficients(degree + 1), m_companionMatrix(degree, degree) {
	m_polynomialDegree = degree;
	InitCompanionMatrix();
}

void CompanionMatrixRootsFinder::InitCompanionMatrix()
{
	for (int j = 1; j <= m_polynomialDegree; j++)
	{
		for (int k = 1; k <= m_polynomialDegree; k++)
		{
			if (j == 1)
			{
				m_companionMatrix(j - 1, k - 1) = Delta(2, k);
			}
			else if (j < m_polynomialDegree)
			{
				m_companionMatrix(j - 1, k - 1) = 0.5 * (Delta(j, k + 1) + Delta(j, k - 1));
			}
		}
	}
}


/// <summary>
/// Eq.18: Compute the companion matrix
/// </summary>
void CompanionMatrixRootsFinder::computeCompanionMatrix()
{
	for (int k = 1; k <= m_polynomialDegree; k++)
	{
		m_companionMatrix(m_polynomialDegree - 1, k - 1) = -(m_coefficients(k - 1) / (double)(2 * m_coefficients(m_polynomialDegree))) + 0.5 * Delta(m_polynomialDegree - 1, k);

	}
}

/// <summary>
/// Eq.13: Get Pj value where:
/// Pj = 2 if j = 0\N or
///		 1 otherwise
/// </summary>
/// <param name="j"></param>
/// <returns>Pj</returns>
int CompanionMatrixRootsFinder::getPj(int j)
{
	return j == 0 || j == (m_polynomialDegree - 1)? 2 : 1;
}
/// <summary>
/// Eq.19: Binary function, 
/// return 1 if r==q or 0 otherwise
/// </summary>
/// <param name="q"></param>
/// <param name="r"></param>
/// <returns></returns>
int  CompanionMatrixRootsFinder::Delta(int q, int r)
{
	return q == r ? 1 : 0;
}

/// <summary>
/// Get the rootes of the fitted CPP
/// The roots are the companion matrix eigenvalues
/// </summary>
/// <returns></returns>
VectorXd CompanionMatrixRootsFinder::getRoots()
{
	//vector<double> eigenvalues = ; // Compute the eigenvalues
	EigenSolver<Eigen::MatrixXd> solver(m_companionMatrix);
	VectorXcd eigenvalues = solver.eigenvalues();
	VectorXd temp = VectorXd(eigenvalues.size());
	int index = 0;
	for (int i = 0; i < eigenvalues.size(); i++)
	{
		if (eigenvalues(i).imag() == 0 && eigenvalues(i).real() <= 1 && eigenvalues(i).real() >= -1)
		{
			temp(index++) = eigenvalues(i).real();
		}
	}
	VectorXd result = VectorXd(index);
	for (int i = 0; i < index; i++)
	{
		result(i) = temp(i);
	}
	return result;
}

int CompanionMatrixRootsFinder::findRoots(double* coefficients, int degree, double* roots)
{
	int numberOfRoots = 0;
	if (nullptr != coefficients && nullptr != roots && m_polynomialDegree == degree)
	{
		for (int i = 0; i <= m_polynomialDegree; i++)
		{
			m_coefficients(i) = coefficients[i];
		}
		computeCompanionMatrix();
		VectorXd result = getRoots();
		numberOfRoots = result.size();
		for (int i = 0; i < numberOfRoots; i++)
		{
			roots[i] = result(i);
		}
	}
	return numberOfRoots;
}
