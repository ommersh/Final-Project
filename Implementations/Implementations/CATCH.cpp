#include "CATCH.h"

extern double getCurrentTymeInMicroSec();

/// <summary>
/// Alg.2: Get the TCA usnig the catch algorithm
/// </summary>
/// <param name="f1">Location in time of the first object(x,y,z)</param>
/// <param name="f2">Location in time of the second object(x,y,z)</param>
/// <param name="Gamma">The time interval size</param>
/// <param name="t_max">the given function latest time(from 0 to t max)</param>
/// <returns></returns>
TCA CATCH::CatchAlgorithm(sPointData* pointsInTime, double *timePoints, int lastPointIndex)
{

	double fd[N+1], fx[N + 1], fy[N + 1], fz[N + 1];
	TCA tca;
	CPP FdCpp,xCpp,yCpp,zCpp;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	tca.time = 0;
	double a = 0;
	double b = timePoints[N];
	int startPointIndex, endPointIndex;
	startPointIndex = 0;
	endPointIndex = N;
	double time;
	double dist;
	long double tempX;
	Vector3d v1;
	VectorXd Tau;
	int roundNumber = 0;
	int offset;
	while (endPointIndex <= lastPointIndex)
	{
		a = timePoints[startPointIndex];
		b = timePoints[endPointIndex];
		offset = (N)*roundNumber;
		//Calculate Fd for the N current points
		for (int i = 0; i <= N; i++)
		{
			fx[i] = pointsInTime[offset + i].r1x - pointsInTime[offset + i].r2x;
			fy[i] = pointsInTime[offset + i].r1y - pointsInTime[offset + i].r2y;
			fz[i] = pointsInTime[offset + i].r1z - pointsInTime[offset + i].r2z;
			fd[i] = 2 * (((fx[i])* (pointsInTime[offset + i].v1x - pointsInTime[offset + i].v2x)) +
						((fy[i]) * (pointsInTime[offset + i].v1y - pointsInTime[offset + i].v2y)) +
						((fz[i]) * (pointsInTime[offset + i].v1z - pointsInTime[offset + i].v2z)));
		}
		FdCpp.fitCPP(a, b, fd);
		//get the roots
		Tau = FdCpp.getRoots();
		TauSize = Tau.size();
		//fit cpp to x\y\z
		xCpp.fitCPP(a, b, fx);
		yCpp.fitCPP(a, b, fy);
		zCpp.fitCPP(a, b, fz);
		for (int i = 0; i < TauSize; i++)
		{
			tempX = ((b + a) / 2 + Tau[i] * (b - a) / 2);
			v1(0) = xCpp.getValue(tempX);
			v1(1) = yCpp.getValue(tempX);
			v1(2) = zCpp.getValue(tempX);
			dist = abs(v1.norm());
			if (dist < tca.distance)
			{
				tca.distance = dist;
				tca.time = ((b + a) / 2 + Tau[i] * (b - a) / 2);
			}
		}
		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + N;
		roundNumber++;
	}
	return tca;
}

CPP::CPP() : coefficients(N + 1), interpolationMatrix(N + 1, N + 1), companionMatrix(N, N) {
	calculateInterpolationMatrix();
	InitCompanionMatrix();
}

void CPP::InitCompanionMatrix()
{
	for (int j = 1; j <= N; j++)
	{
		for (int k = 1; k <= N; k++)
		{
			if (j == 1)
			{
				companionMatrix(j - 1, k - 1) = Delta(2, k);
			}
			else if (j < N)
			{
				companionMatrix(j - 1, k - 1) = 0.5 * (Delta(j, k + 1) + Delta(j, k - 1));
			}
		}
	}
}

/// <summary>
/// Alg.1: Fit a CPP in interval where N is constant 
/// </summary>
/// <param name="intervalStart"></param>
/// <param name="intervalEnd"></param>
/// <param name="g"></param>
void CPP::fitCPP(double intervalStart, double intervalEnd, double* g)
{
	m_intervalStart = intervalStart;
	m_intervalEnd = intervalEnd;
	//Eq.14/15
	for (int j = 0; j <= N; j++)
	{
		double sum = 0;
		for (int k = 0; k <= N; k++)
		{
			sum += interpolationMatrix(j, k) * g[N - k];
		}
		coefficients(j) = sum;
	}
	computeCompanionMatrix();
}

/// <summary>
/// Eq.9: Using the fitted CPP get a function value at X
/// g(x) = gN(x) = sum from j = 0 to N of
/// aj Tj(2x - b + a / b - a)
/// </summary>
/// <param name="x"></param>
/// <returns></returns>
double CPP::getValue(double x)
{
	double result = 0;
	for (int i = 0; i <= N; i++)
	{
		result += coefficients(i) * getTj((2 * x - m_intervalEnd - m_intervalStart) / (m_intervalEnd - m_intervalStart),i);
	}
	return result;
}

/// <summary>
/// Eq.10: Calculate Tj
/// Where Tj(x) = cos(j arccos(x))
/// </summary>
/// <param name="x"></param>
/// <param name="j"></param>
/// <returns></returns>
double CPP::getTj(double x,int j)
{
	return cos(j * acos(x));
}

/// <summary>
/// Eq.12: calculate the interpolation matrix
/// where Mj,k = 2/PjPkN cos(j pi k/N)
/// </summary>
void CPP::calculateInterpolationMatrix()
{
	for (int j = 0; j <= N; j++)
	{
		for (int k = 0; k <= N; k++)
		{
			interpolationMatrix(j,k) = 2 / (double)(getPj(j) * getPj(k) * N) * cos(j * k * pi/N);

		}
	}
}

/// <summary>
/// Eq.11: Get the value Xj
/// where Xj = b-a/2 * cos(pi j/N) + b+a/2
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="j"></param>
/// <returns></returns>
double CPP::getX(double a, double b,int j)
{
	return ((b - a) / 2)*cos(pi * j / N) + ((b + a) / 2);
}

/// <summary>
/// Eq.18: Compute the companion matrix
/// </summary>
void CPP::computeCompanionMatrix()
{
	for (int k = 1; k <= N; k++)
	{
		companionMatrix(N - 1, k - 1) = -(coefficients(k - 1) / (double)(2 * coefficients(N))) + 0.5 * Delta(N - 1, k);

	}
}
/// <summary>
/// Eq.13: Get Pj value where:
/// Pj = 2 if j = 0\N or
///		 1 otherwise
/// </summary>
/// <param name="j"></param>
/// <returns>Pj</returns>
int CPP::getPj(int j)
{
	return j == 0 || j == (N -1)? 2 : 1;
}
/// <summary>
/// Eq.19: Binary function, 
/// return 1 if r==q or 0 otherwise
/// </summary>
/// <param name="q"></param>
/// <param name="r"></param>
/// <returns></returns>
int  CPP::Delta(int q, int r)
{
	return q == r ? 1 : 0;
}

/// <summary>
/// Get the rootes of the fitted CPP
/// The roots are the companion matrix eigenvalues
/// </summary>
/// <returns></returns>
VectorXd CPP::getRoots()
{
	//vector<double> eigenvalues = ; // Compute the eigenvalues
	EigenSolver<Eigen::MatrixXd> solver(companionMatrix);
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