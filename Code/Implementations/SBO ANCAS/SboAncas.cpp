#include "SboAncas.h"



void SboAncas::init(ISinglePointPropogator* propogator,double TOLd,double TOLt)
{
	m_propogator = propogator;
	m_TOLd = TOLd;
	m_TOLt = TOLt;
}



/// <summary>
/// The SboAncas algorithm
/// </summary>
/// <param name="pointsInTime">
/// Points in time(r1,r2,v1,v2)
/// </param>
/// <param name="timePoints">
/// The time points, respective times for the data points
/// </param>
/// <param name="lastPointIndex">
///	The index of the last point(data + time) 
/// </param>
/// <returns>
/// TCA, time of closest approach and the corresponding distance
/// </returns>
TCA SboAncas::runAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex)
{
	TCA tca;
	TCA tempTca;

	tca.time = 0;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	tca.numberOfPoints = 0;
	//1. Prepare the variables
	
	int startPointIndex, endPointIndex;
	int roundNumber = 0;
	int offset = 0;
	int numberOfPoints = 1;

	//inner loop temp variables
	double rd;
	double tm;
	bool innerLoopCondition;
	bool timeToleranceReached;;
	bool distToleranceReached;;
	int tmIndex;
	TcaCalculation::sPointData tmData;

	startPointIndex = 0;
	endPointIndex = 3;
	//run over all the data
	while (endPointIndex <= lastPointIndex)
	{
		offset = (3)*roundNumber;
		numberOfPoints += 3;

		//initialize the data point->tnew
		for (int i = 0; i < 4; i++)
		{
			m_dataPoints[i] = pointsInTime[offset + i];
		}
		//Inner Loop
		innerLoopCondition = true;
		timeToleranceReached = false;
		do
		{
			tempTca = ANCASIteration();
			//get rd and tm
			rd = tempTca.distance;
			tm = tempTca.time;
			//sample the point
			tmData = m_propogator->getSinglePoint(tm);
			//get rt(tm)
			tempTca.distance = sqrt(pow((tmData.r1x - tmData.r2x), 2) + pow((tmData.r1y - tmData.r2y), 2) + pow((tmData.r1z - tmData.r2z), 2));

			//check the conditions
			if (fabs(tempTca.distance - rd) < m_TOLd)
			{
				distToleranceReached = true;
			}

			tmIndex = 0;
			for (int i = 0; i < 4; i++)
			{
				//check the time tolerance
				if (fabs(m_dataPoints[i].time - tm) < m_TOLt)
				{
					timeToleranceReached = true;
				}
				//check the location of tm in the array
				if (tm > m_dataPoints[i].time)
				{
					tmIndex++;
				}
			}
			//update tnew
			// we will remove the first or the last point in the array
			// keeping the closest
			int srcIndex = 0;
			int dstIndex = 0;

			if (fabs(tm - m_dataPoints[0].time) > fabs(tm - m_dataPoints[0].time))
			{
				// We are keeping the last point
				srcIndex = 1;

			}
			while ((dstIndex < 4)&& (srcIndex < 4))
			{
				if (dstIndex == tmIndex)
				{
					dstIndex++;
				}
				m_dataPoints[dstIndex++] = m_dataPoints[srcIndex++];
			}
			m_dataPoints[tmIndex] = tmData;

			//check the end condition
			if (false == distToleranceReached && false == timeToleranceReached)
			{
				innerLoopCondition = false;
			}
		} while (innerLoopCondition);
		
		if (tempTca.distance < tca.distance)
		{
			tca.distance = tempTca.distance;
			tca.time = tempTca.time;
		}
		
		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + 3;
		roundNumber++;
	}


	tca.numberOfPoints = numberOfPoints;
	
	return tca;
}
