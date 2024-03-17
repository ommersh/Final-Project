#ifndef SHIELD_SBOANCAS_H    // Check if the symbol SHIELD_SBOANCAS_H is not defined
#define SHIELD_SBOANCAS_H    // Define the symbol SHIELD_SBOANCAS_H



#include "ANCAS.h"
#include "ISinglePointPropogator.h"

/// <summary>
/// Implentation of ANCAS(Alfano\Negron Close Approach Software) - based on Determining Satellite Close Approaches,Part 2 by Salvatore Alfano
/// 
/// </summary>
class SboAncas : public ANCAS
{
public:
	void init(ISinglePointPropogator* propogator, double TOLd, double TOLt);

	virtual ~SboAncas()
	{

	};
	TCA runAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex);

protected:
	ISinglePointPropogator* m_propogator;
	double m_TOLd;
	double m_TOLt;

};



#endif //SHIELD_SBOANCAS_H