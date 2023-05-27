#include "CATCH.h"



/// <summary>
/// Alg.2: Get the TCA usnig the catch algorithm
/// </summary>
/// <param name="f1">Location in time of the first object(x,y,z)</param>
/// <param name="f2">Location in time of the second object(x,y,z)</param>
/// <param name="Gamma">The time interval size</param>
/// <param name="t_max">the given function latest time(from 0 to t max)</param>
/// <returns></returns>
TCA CATCH::CatchAlgorithm(VectorFunction* locationInTimeObject1, VectorFunction* locationInTimeObject2,
	VectorFunction* velocityInTimeObject1, VectorFunction* velocityInTimeObject2, double Gamma, double t_max)
{
	RelativeDistanceFunction relativeVelocity(velocityInTimeObject1, velocityInTimeObject2);
	RelativeDistanceFunction relativeLocation(locationInTimeObject1, locationInTimeObject2);
	Fd fd = Fd(&relativeLocation, &relativeVelocity);
	RelativeFunctionInIndex relativeLocationX(locationInTimeObject1, locationInTimeObject2,0);
	RelativeFunctionInIndex relativeLocationY(locationInTimeObject1, locationInTimeObject2, 1);
	RelativeFunctionInIndex relativeLocationZ(locationInTimeObject1, locationInTimeObject2, 2);
	TCA tca;
	CPP FdCpp,xCpp,yCpp,zCpp;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	tca.time = 0;
	double a = 0;
	double b = Gamma;
	double time;
	double dist;
	Vector3d v1;
	VectorXd Tau;
	while (b <= t_max)
	{
		FdCpp.fitCPP(a,b, &fd);
		//get the roots
		Tau = FdCpp.getRoots();
		TauSize = Tau.size();
		//fit cpp to x\y\z
		xCpp.fitCPP(a, b, &relativeLocationX);
		yCpp.fitCPP(a, b, &relativeLocationY);
		zCpp.fitCPP(a, b, &relativeLocationZ);
		for (int i = 0; i < TauSize; i++)
		{
			std::cout << "Tau " << i << ":" << Tau[i] * b << "\n";
			std::cout << "FdCpp " << Tau[i] * b << ":" << FdCpp.getValue(Tau[i]) << "\n";
			v1(0) = xCpp.getValue(Tau[i]);
			v1(1) = yCpp.getValue(Tau[i]);
			v1(2) = zCpp.getValue(Tau[i]);
			dist = abs(v1.norm());
			if (dist < tca.distance)
			{
				tca.distance = dist;
				tca.time = Tau[i] * b;
			}
		}
		a = b;
		b += Gamma;
	}
	return tca;
}

/// <summary>
/// Alg.1: Fit a CPP in interval where N is constant 
/// </summary>
/// <param name="intervalStart"></param>
/// <param name="intervalEnd"></param>
/// <param name="g"></param>
void CPP::fitCPP(double intervalStart, double intervalEnd, Function<double>* g)
{
	m_intervalStart = intervalStart;
	m_intervalEnd = intervalEnd;
	calculateInterpolationMatrix();
	//Eq.14/15
	for (int j = 0; j <= N; j++)
	{
		double sum = 0;
		for (int k = 0; k <= N; k++)
		{
			//std::cout <<" X "<< k << " = "<< getX(intervalStart, intervalEnd, k) << " gxj ="<< g->getValue(N - k) << "\n";
			sum += interpolationMatrix(j, k) * g->getValue(N - k);
		}
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
			//std::cout << interpolationMatrix(j, k) << " at " << j << " " << k << "\n";
			//std::cout << cos(j * k * pi / N)<< " \n";
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
	for (int j = 0; j < N; j++)
	{
		for (int k = 0; k < N; k++)
		{
			if (j == 1)
			{
				companionMatrix(j,k) = Delta(1, k);
			}
			else if (j < (N -1))
			{
				companionMatrix(j,k) = 0.5 * (Delta(j, k + 1) + Delta(j, k - 1));
			}
			else
			{
				companionMatrix(j,k) = -(coefficients(k) / (double)(2 * coefficients(N)) + 0.5 * Delta(N - 2, k));
			}
		}
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
	return j == 0 || j == N ? 2 : 1;
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
	//std::cout << eigenvalues << " \n";
	return eigenvalues.real();
}
