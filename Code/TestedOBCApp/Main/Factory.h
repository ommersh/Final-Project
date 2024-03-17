#ifndef SHIELD_Factory_H    // Check if the symbol SHIELD_Factory_H is not defined
#define SHIELD_Factory_H    // Define the symbol SHIELD_Factory_H

#include "ITimer.h"
#include "ITcaAlgorithm.h"
#include "IRootsFindAlg.h"
#include "ISinglePointPropogator.h"

#include "ChronoBasedTimer.h"
#include "CATCH.h"
#include "ANCAS.h"
#include "SboAncas.h"

#include "CompanionMatrixRootsFinderEigen.h"
//#include "CompanionMatrixRootsFinderArmadillo.h"
#include "SGP4SinglePointGenerator.h"

#include "CommonStructures.h"
#include "AppConfigurationManager.h"
#include "TestManager.h"

/// <summary>
/// TestedOBCApp Factory.
/// Create and manage app opbjects.
/// Singleton.
/// </summary>
class Factory
{
public:
	/// <summary>
	/// Get a reference to the single instance of the factory
	/// </summary>
	/// <returns></returns>
	static Factory* getReference();
	~Factory();
	/// <summary>
	/// Get the timer, for timing tests.
	/// </summary>
	/// <returns></returns>
	ITimer*			getTimer();
	/// <summary>
	/// Get appropriate IRootsFindAlg object based on the requested type;
	/// </summary>
	/// <param name="algType"></param>
	/// <param name="degree"></param>
	/// <returns></returns>
	IRootsFindAlg*	getRootsFindAlg(TestParameters::CatchRootsAlg algType, int degree);
	/// <summary>
	/// Get Catch object initialize to the requested degree and root finding algorithm.
	/// </summary>
	/// <param name="algType"></param>
	/// <param name="degree"></param>
	/// <returns></returns>
	CATCH*			getCATCH(TestParameters::CatchRootsAlg algType, int degree);
	/// <summary>
	/// Get Ancas object, should be initialized.
	/// </summary>
	/// <returns></returns>
	ANCAS*			getANCAS();

	SboAncas*		getSboAncas(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2, double TOLd, double TOLt);

	ISinglePointPropogator* getSinglePointPropogator(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2);
	
private:
	static Factory* m_reference;
	Factory();
	ITimer* m_timer;
	CATCH m_catch;
	ANCAS m_ancas;
	SboAncas m_sboAncas;
	CompanionMatrixRootsFinderEigen m_companionMatrixRootsFinderEigen;
	//CompanionMatrixRootsFinderArmadillo m_companionMatrixRootsFinderAArmadillo;
	SGP4SinglePointGenerator m_SGP4SinglePointGenerator;
};









#endif //SHIELD_Factory_H