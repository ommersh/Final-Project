#ifndef SHIELD_SGP4SinglePointGenerator_H    // Check if the symbol SHIELD_SGP4SinglePointGenerator_H is not defined
#define SHIELD_SGP4SinglePointGenerator_H    // Define the symbol SHIELD_SGP4SinglePointGenerator_H



#include "ISinglePointPropogator.h"
#include "SGP4.h"

/// <summary>
/// Implentation of ANCAS(Alfano\Negron Close Approach Software) - based on Determining Satellite Close Approaches,Part 2 by Salvatore Alfano
/// 
/// </summary>
class SGP4SinglePointGenerator : public ISinglePointPropogator
{
public:
	virtual ~SGP4SinglePointGenerator()
	{

	};
	virtual void Init(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1Min, double startTime2Min);
	virtual TcaCalculation::sPointData getSinglePoint(double timePointSec) override;

protected:
	elsetrec m_elsetrec1;
	elsetrec m_elsetrec2;
	double m_startTime1Min;
	double m_startTime2Min;
};



#endif //SHIELD_SGP4SinglePointGenerator_H