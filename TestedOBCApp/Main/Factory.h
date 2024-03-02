#ifndef SHIELD_Factory_H    // Check if the symbol SHIELD_Factory_H is not defined
#define SHIELD_Factory_H    // Define the symbol SHIELD_Factory_H

#include "ITimer.h"
#include "ITcaAlgorithm.h"
#include "IRootsFindAlg.h"

#include "ChronoBasedTimer.h"
#include "CATCH.h"
#include "ANCAS.h"

#include "CompanionMatrixRootsFinderEigen.h"
//#include "CompanionMatrixRootsFinderArmadillo.h"

#include "TestDefinitions.h"
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

	
private:
	static Factory* m_reference;
	Factory();
	ITimer* m_timer;
	CATCH m_catch;
	ANCAS m_ancas;

	CompanionMatrixRootsFinderEigen m_companionMatrixRootsFinderEigen;
	//CompanionMatrixRootsFinderArmadillo m_companionMatrixRootsFinderAArmadillo;
};









#endif //SHIELD_Factory_H