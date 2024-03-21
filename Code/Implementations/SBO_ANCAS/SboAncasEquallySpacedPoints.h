#ifndef SHIELD_SboAncasEquallySpacedPoints_H    // Check if the symbol SHIELD_SboAncasEquallySpacedPoints_H is not defined
#define SHIELD_SboAncasEquallySpacedPoints_H    // Define the symbol SHIELD_SboAncasEquallySpacedPoints_H



#include "SboAncas.h"
#include "ISinglePointPropogator.h"

/// <summary>
/// Implentation of ANCAS(Alfano\Negron Close Approach Software) - based on Determining Satellite Close Approaches,Part 2 by Salvatore Alfano
/// 
/// </summary>
class SboAncasEquallySpacedPoints : public SboAncas
{
public:
	virtual ~SboAncasEquallySpacedPoints()
	{

	};
	TCA runAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex) override;

};



#endif //SHIELD_SboAncasEquallySpacedPoints_H