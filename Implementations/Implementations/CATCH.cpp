#include "CATCH.h"
CATCH::CATCH(int order,double maxError)
{
	Order = order;
	MaxError = maxError;

}

int p(int j)
{
	return j == 0 || j == N ? 2 : 1;
}

int Delta(int q, int r)
{
	return q == r ? 1 : 0;
}

TCA CATCH::CatchAlgorithm(definedFunctionInInterval g,int Gamma,int t_max)
{
	//Tau
	TCA tca;
	tca.distance = std::numeric_limits<double>::max();//init the distance to inf
	tca.time = 0;
	int a = 0;
	int b = Gamma;
	double time;
	double dist;
	while (b <= t_max)
	{
		fitCPP(a,b, g,0,16);//temp
		computeCompanionMatrix();
		getEigenvalues();
		//fit cpp to x\y\z
		for (int i = 0; i < TauSize; i++)
		{
			dist = calculateDistance();
			if (dist < tca.distance)
			{
				tca.distance = dist;
				tca.time = Tau[i];
			}
		}
		a = b;
		b += Gamma;
	}
	return tca;
}

void CATCH::fitCPP(int intervalStart, int intervalEnd, definedFunctionInInterval g, double MaxError, int Order)
{
	double interpolationMatrix[N+1][N+1];
	calculateInterpolationMatrix(interpolationMatrix);
	for (int j = 0; j <= N; j++)
	{
		double sum = 0;
		for (int k = 0; k <= N; k++)
		{
			sum += interpolationMatrix[j][k] * g.getValue(getX(intervalStart, intervalEnd,k));
		}
		a[j] = sum;
	}
	computeCompanionMatrix();
	getEigenvalues();
}
void CATCH::calculateInterpolationMatrix(double interpolationMatrix[N + 1][N + 1])
{
	for (int j = 0; j <= N; j++)
	{
		for (int k = 0; k <= N; k++)
		{
			interpolationMatrix[j][k] = 2 / (p(j) * p(k) * N) * cos(j * k * pi);
		}
	}
}

double CATCH::getX(double a, double b,int j)
{
	return ((b - a) / 2)*cos(pi * j / N) + ((b + a) / 2);
}
void CATCH::computeCompanionMatrix()
{
	for (int j = 1; j <= N; j++)
	{
		for (int k = 1; k <= N; k++)
		{
			if (j == 1)
			{
				CompanionMatrix[j - 1][k - 1] = Delta(2, k);
			}
			else if (j < N)
			{
				CompanionMatrix[j - 1][k - 1] = 0.5 * (Delta(j, k + 1) + Delta(j, k - 1));
			}
			else
			{
				CompanionMatrix[j - 1][k - 1] = -(a[k - 1] / (2 * a[N]) + 0.5 * Delta(N - 1, k));
			}
		}
	}
}

