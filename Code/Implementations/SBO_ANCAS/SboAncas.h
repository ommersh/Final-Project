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
	void Init(ISinglePointPropogator* propogator, double TOLd, double TOLt);

	virtual ~SboAncas()
	{

	};
	virtual TCA RunAlgorithm(TcaCalculation::sPointData* pointsInTime, int lastPointIndex);

protected:
	ISinglePointPropogator* m_propogator;
	double m_TOLd;
	double m_TOLt;
	TcaCalculation::sPointData m_tempDataPoints[4];

};



#endif //SHIELD_SBOANCAS_H