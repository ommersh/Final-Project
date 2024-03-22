#include "SGP4SinglePointGenerator.h"

void SGP4SinglePointGenerator::init(elsetrec elsetrec1, elsetrec elsetrec2,double startTime1Min, double startTime2Min)
{
	m_elsetrec1 = elsetrec1;
	m_elsetrec2 = elsetrec2;
	m_startTime1Min = startTime1Min;
	m_startTime2Min = startTime2Min;

}
TcaCalculation::sPointData SGP4SinglePointGenerator::getSinglePoint(double timePointSec)
{
	TcaCalculation::sPointData pointData;
	double r1[3], v1[3];
	double r2[3], v2[3];
	double timePointMin = timePointSec / 60;
	// Compute position and velocity
	SGP4Funcs::sgp4(m_elsetrec1, m_startTime1Min + timePointMin, r1, v1);
	SGP4Funcs::sgp4(m_elsetrec2, m_startTime2Min + timePointMin, r2, v2);

	//save the results
	pointData.time = timePointSec;

	pointData.r1x = r1[0];
	pointData.r1y = r1[1];
	pointData.r1z = r1[2];

	pointData.v1x = v1[0];
	pointData.v1y = v1[1];
	pointData.v1z = v1[2];

	pointData.r2x = r2[0];
	pointData.r2y = r2[1];
	pointData.r2z = r2[2];

	pointData.v2x = v2[0];
	pointData.v2y = v2[1];
	pointData.v2z = v2[2];

	return pointData;
}
