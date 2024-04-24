#ifndef SHIELD_TcaCalculation_H    // Check if the symbol SHIELD_TcaCalculation_H is not defined
#define SHIELD_TcaCalculation_H    // Define the symbol SHIELD_TcaCalculation_H
#include <cstdint>

static const int CATCH_MAX_DEGREE = 32;

namespace TcaCalculation
{

	/// <summary>
	/// Time of Closest Approach (TCA)
	/// </summary>
	struct TCA {
		double time;
		double distance;
		int32_t numberOfPoints;
	};


	/// <summary>
	/// Struct contains set of location and velocity values for 2 objects in a single point in time
	/// </summary>
	struct sPointData {
		double r1x;
		double r1y;
		double r1z;
		double r2x;
		double r2y;
		double r2z;
		double v1x;
		double v1y;
		double v1z;
		double v2x;
		double v2y;
		double v2z;
		double time;
	};

};
#endif //SHIELD_TcaCalculation_H