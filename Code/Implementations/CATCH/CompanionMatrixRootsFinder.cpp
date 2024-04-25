#include "CompanionMatrixRootsFinder.h"



CompanionMatrixRootsFinder::CompanionMatrixRootsFinder(){

}
void CompanionMatrixRootsFinder::Init(int degree)
{
	m_polynomialDegree = degree;
	InitCompanionMatrix();
}

void CompanionMatrixRootsFinder::InitCompanionMatrix()
{
	for (int j = 0; j < m_polynomialDegree; j++)
	{
		for (int k = 0; k < m_polynomialDegree; k++)
		{
			if (j == 0)
			{
				m_companionMatrix[j][k] = Delta(1, k);
			}
			else if (j < (m_polynomialDegree - 1))
			{
				m_companionMatrix[j][k] = 0.5 * (Delta(j, k + 1) + Delta(j, k - 1));
			}
			else
			{
				m_companionMatrix[j][k] = 0;
			}
		}
	}
}

/// <summary>
/// Eq.18: Compute the companion matrix
/// </summary>
void CompanionMatrixRootsFinder::computeCompanionMatrix()
{
	for (int k = 0; k < m_polynomialDegree; k++)
	{
		m_companionMatrix[m_polynomialDegree - 1][k] = -(m_coefficients[k] / (double)(2 * m_coefficients[m_polynomialDegree])) + 0.5 * Delta(m_polynomialDegree - 2, k);
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


int CompanionMatrixRootsFinder::FindRoots(double* coefficients, int degree, double* roots)
{
	int numberOfRoots = 0;
	if (nullptr != coefficients && nullptr != roots && m_polynomialDegree == degree)
	{
		for (int i = 0; i <= m_polynomialDegree; i++)
		{
			m_coefficients[i] = coefficients[i];
		}
		computeCompanionMatrix();
		numberOfRoots = findEigenValues();
		for (int i = 0; i < numberOfRoots; i++)
		{
			roots[i] = m_eigenValues[i];
		}
	}
	return numberOfRoots;
}
