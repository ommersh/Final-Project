#ifndef SHIELD_TCA_Calculation_H    // Check if the symbol SHIELD_TCA_Calculation_H is not defined
#define SHIELD_TCA_Calculation_H    // Define the symbol SHIELD_TCA_Calculation_H


namespace TCA_Calculation
{
	/// <summary>
	/// Time of Closest Approach (TCA)
	/// </summary>
	struct TCA {
		long double time;
		long double distance;
	};
	
};

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
};

#endif //SHIELD_TCA_Calculation_H