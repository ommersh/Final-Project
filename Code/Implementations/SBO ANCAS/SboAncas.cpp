#include "SboAncas.h"

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
		do
		{
			tempTca = ANCASIteration();
			//calculate rd

			//sample the point

			//update tnew

			//check the end condition
		} while (true);
		/*
		tempTca = ANCASIteration();
		if (tempTca.distance < tca.distance)
		{
			tca.distance = tempTca.distance;
			tca.time = tempTca.time;
		}
		*/
		startPointIndex = endPointIndex;
		endPointIndex = endPointIndex + 3;
		roundNumber++;
	}


	tca.numberOfPoints = numberOfPoints;
	
	return tca;
}
