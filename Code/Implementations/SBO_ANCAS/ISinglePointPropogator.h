#ifndef SHIELD_ISinglePointPropogator_H    // Check if the symbol SHIELD_ISinglePointPropogator_H is not defined
#define SHIELD_ISinglePointPropogator_H    // Define the symbol SHIELD_ISinglePointPropogator_H



#include "TcaCalculation.h"


/// <summary>
/// Implentation of ANCAS(Alfano\Negron Close Approach Software) - based on Determining Satellite Close Approaches,Part 2 by Salvatore Alfano
/// 
/// </summary>
class ISinglePointPropogator
{
public:
	virtual ~ISinglePointPropogator()
	{

	};
	virtual TcaCalculation::sPointData getSinglePoint(double timePoint) = 0;

protected:

};



#endif //SHIELD_ISinglePointPropogator_H