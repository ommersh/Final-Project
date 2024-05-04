#include "SboAncas.h"



void SboAncas::Init(ISinglePointPropogator* propogator,double TOLd,double TOLt)
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
TCA SboAncas::RunAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex)
{
	TCA tca = { 0 };
	TCA iterationTca;
	TCA ancasTca;

	tca.time = 0;
	tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	iterationTca.time = 0;
	iterationTca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
	tca.numberOfPoints = 0;
	//1. Prepare the variables
	
	int startPointIndex, endPointIndex;
	int roundNumber = 0;
	int offset = 0;
	int numberOfPoints = 1;

	//inner loop temp variables
	double rd = std::numeric_limits<double>::max();
	double tm = 0;
	bool innerLoopCondition = true;
	bool timeToleranceReached = false;
	bool distToleranceReached = false;
	int tmIndex = 0;
	TcaCalculation::sPointData tmData;

	startPointIndex = 0;
	endPointIndex = 3;
	//run over all the data
	while (endPointIndex < lastPointIndex)
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
		distToleranceReached = false;
		do
		{
			ancasTca = ANCASIteration();
			//in root was found - give up
			if (ancasTca.time == -1)
			{
				innerLoopCondition = false;
				/**.numberOf_NoRootsFound++;
				if (distToleranceReached)
					tca.numberOf_ToldReached++;
				if (timeToleranceReached)
					tca.numberOf_ToltReached++;*/
			}
			else
			{
				//get rd and tm
				rd = ancasTca.distance;
				tm = ancasTca.time;
				//sample the point
				tmData = m_propogator->getSinglePoint(tm);
				numberOfPoints++;
				//get rt(tm)
				iterationTca.distance = sqrt(pow((tmData.r1x - tmData.r2x), 2) + pow((tmData.r1y - tmData.r2y), 2) + pow((tmData.r1z - tmData.r2z), 2));
				iterationTca.time = tm;
				//check the conditions
				if (fabs(iterationTca.distance - rd) < m_TOLd)
				{
					distToleranceReached = true;
				}

				timeToleranceReached = true;

				for (int i = 0; i < 4; i++)
				{
					//check the time tolerance
					if (fabs(m_dataPoints[i].time - tm) >= m_TOLt)
					{
						timeToleranceReached = false;
					}

				}
				//update tnew
				// we will remove the first or the last point in the array
				// keeping the closest
				int srcIndex = 0;
				int dstIndex = 0;
				tmIndex = 0;

				if (fabs(tm - m_dataPoints[0].time) > fabs(tm - m_dataPoints[3].time))
				{
					// We are keeping the last point
					srcIndex = 1;
					for (int i = 1; i < 4; i++)
					{
						//check the location of tm in the array
						if (tm > m_dataPoints[i].time)
						{
							tmIndex++;
						}
					}
				}
				else
				{
					// We are keeping the first point
					for (int i = 0; i < 3; i++)
					{
						//check the location of tm in the array
						if (tm > m_dataPoints[i].time)
						{
							tmIndex++;
						}
					}
				}
				while ((dstIndex < 4) && (srcIndex < 4))
				{
					if (dstIndex == tmIndex)
					{
						dstIndex++;
						m_tempDataPoints[tmIndex] = tmData;
					}
					else
					{
						m_tempDataPoints[dstIndex++] = m_dataPoints[srcIndex++];
					}
				}
				for (int i = 0; i < 4; i++)
				{
					m_dataPoints[i] = m_tempDataPoints[i];
				}
				//check the end condition
				if (true == distToleranceReached && true == timeToleranceReached)
				{
					innerLoopCondition = false;
					//tca.numberOf_ToldReached++;
					//tca.numberOf_ToltReached++;
					//tca.numberOf_FullTolReached++;
				}
			}
		} while (innerLoopCondition);

		if (iterationTca.distance < tca.distance)
		{
			tca.distance = iterationTca.distance;
			tca.time = iterationTca.time;
		}

		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + 3;
		roundNumber++;
	}


	tca.numberOfPoints = numberOfPoints;
	
	return tca;
}
