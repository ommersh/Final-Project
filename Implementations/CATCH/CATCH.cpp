#include "CATCH.h"





CATCH::CATCH()
{
	m_degree = 0;
	m_rootsFinder = nullptr;
}

// Destructor
CATCH::~CATCH() {

}

void CATCH::init(IRootsFindAlg* rootsFinder, int degree)
{
	m_degree = degree;
	m_rootsFinder = rootsFinder;
	rootsFinder->init(degree);
	m_FdCpp.init(m_degree);
	m_xCpp.init(m_degree);
	m_yCpp.init(m_degree);
	m_zCpp.init(m_degree);
}


TCA CATCH::runAlgorithm(TcaCalculation::sPointData* pointsInTime, double *timePoints, int lastPointIndex)
{
	TCA tca;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	tca.time = 0;
	tca.numberOfPoints = 0;
	int numberOfRoots = 0;
	double a = 0;
	double b = timePoints[m_degree];
	int startPointIndex = 0;
	int endPointIndex = m_degree;
	double dist;
	long double tempX;
	double v1[3];
	int roundNumber = 0;
	int offset;

	while (endPointIndex <= lastPointIndex)
	{
		a = timePoints[startPointIndex];
		b = timePoints[endPointIndex];
		offset = (m_degree)*roundNumber;
		//Calculate Fd for the N current points
		for (int i = 0; i <= m_degree; i++)
		{
			m_fx[i] = pointsInTime[offset + i].r1x - pointsInTime[offset + i].r2x;
			m_fy[i] = pointsInTime[offset + i].r1y - pointsInTime[offset + i].r2y;
			m_fz[i] = pointsInTime[offset + i].r1z - pointsInTime[offset + i].r2z;
			m_fd[i] = 2 * (((m_fx[i])* (pointsInTime[offset + i].v1x - pointsInTime[offset + i].v2x)) +
						((m_fy[i]) * (pointsInTime[offset + i].v1y - pointsInTime[offset + i].v2y)) +
						((m_fz[i]) * (pointsInTime[offset + i].v1z - pointsInTime[offset + i].v2z)));
		}
		m_FdCpp.fitCPP(a, b, m_fd);
		//get the roots
		numberOfRoots = m_rootsFinder->findRoots(m_FdCpp.coefficients, m_degree, Tau);

		//fit cpp to x\y\z
		m_xCpp.fitCPP(a, b, m_fx);
		m_yCpp.fitCPP(a, b, m_fy);
		m_zCpp.fitCPP(a, b, m_fz);
		for (int i = 0; i < numberOfRoots; i++)
		{
			tempX = ((b + a) / 2 + Tau[i] * (b - a) / 2);
			v1[0] = m_xCpp.getValue(tempX);
			v1[1] = m_yCpp.getValue(tempX);
			v1[2] = m_zCpp.getValue(tempX);
			//calculate the distance
			dist = sqrt(v1[0]* v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
			if (dist < tca.distance)
			{
				tca.distance = dist;
				tca.time = ((b + a) / 2 + Tau[i] * (b - a) / 2);
			}
		}
		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + m_degree;
		roundNumber++;
	}
	tca.numberOfPoints = 1 + offset + m_degree;
	return tca;
}

CPP::CPP() 
{
	m_degree = 0;
}

CPP::~CPP()
{

}

void CPP::init(int degree)
{
	m_degree = degree;
	calculateInterpolationMatrix();
}


/// <summary>
/// Alg.1: Fit a CPP in interval where N is constant 
/// </summary>
/// <param name="intervalStart"></param>
/// <param name="intervalEnd"></param>
/// <param name="g"></param>
void CPP::fitCPP(double intervalStart, double intervalEnd, double g[CATCH_MAX_DEGREE + 1])
{
	m_intervalStart = intervalStart;
	m_intervalEnd = intervalEnd;
	//Eq.14/15
	for (int j = 0; j <= m_degree; j++)
	{
		double sum = 0;
		for (int k = 0; k <= m_degree; k++)
		{
			sum += m_interpolationMatrix[j][k] * g[m_degree - k];
		}
		coefficients[j] = sum;
	}
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
	for (int i = 0; i <= m_degree; i++)
	{
		result += coefficients[i] * getTj((2 * x - m_intervalEnd - m_intervalStart) / (m_intervalEnd - m_intervalStart),i);
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
	for (int j = 0; j <= m_degree; j++)
	{
		for (int k = 0; k <= m_degree; k++)
		{
			m_interpolationMatrix[j][k] = 2 / (double)(getPj(j) * getPj(k) * m_degree) * cos(j * k * pi/ m_degree);

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
	return ((b - a) / 2)*cos(pi * j / m_degree) + ((b + a) / 2);
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
	return j == 0 || j == (m_degree)? 2 : 1;
}
