#ifndef SHIELD_Factory_H    // Check if the symbol SHIELD_Factory_H is not defined
#define SHIELD_Factory_H    // Define the symbol SHIELD_Factory_H

#include "ITimer.h"
#include "ITcaAlgorithm.h"
#include "IRootsFindAlg.h"

#include "WinTimer.h"
#include "CATCH.h"
#include "ANCAS.h"

#include "CompanionMatrixRootsFinderEigen.h"
//#include "CompanionMatrixRootsFinderArmadillo.h"

#include "TestDefinitions.h"
#include "AppConfigurationManager.h"
#include "TestManager.h"

class Factory
{
public:
	static Factory* getReferance();
	~Factory();
	ITimer*			getTimer();
	IRootsFindAlg*	getRootsFindAlg(TestParameters::CatchRootsAlg algType, int degree);
	CATCH*			getCATCH(TestParameters::CatchRootsAlg algType, int degree);
	ANCAS*			getANCAS();

	
private:
	static Factory* m_reference;
	Factory();
	ITimer* m_timer;
	CATCH m_catch;
	ANCAS m_ancas;

	//RootsFindAlg m_rootsFindAlg;
	CompanionMatrixRootsFinderEigen m_companionMatrixRootsFinderEigen;
	//CompanionMatrixRootsFinderArmadillo m_companionMatrixRootsFinderAArmadillo;
};









#endif //SHIELD_Factory_H