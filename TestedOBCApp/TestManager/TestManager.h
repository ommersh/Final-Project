#ifndef SHIELD_TestManager_H    // Check if the symbol SHIELD_TestManager_H is not defined
#define SHIELD_TestManager_H    // Define the symbol SHIELD_TestManager_H

#include "ITimer.h"
#include "TestDefinitions.h"
#include "TCA_Calculation.h"

#include "CATCH.h"
#include "IRootsFindAlg.h"
#include "CompanionMatrixRootsFinder.h"

#include "ANCAS.h"

class TestManager
{
public:
	TestManager(ITimer& timer);
	~TestManager();
	virtual TestResults::TestResult runTest(TestParameters::TestParams params, TcaCalculation::sPointData* pointsData, double* timePoints, int numberOfPopints);
protected:
	IRootsFindAlg* getRootsFindAlg(TestParameters::CatchRootsAlg algType, int degree);
	ANCAS* getAncas();
	CATCH* getCatch(TestParameters::CatchRootsAlg algType, int degree);
	double getCurrentTymeInMicroSec();
	ITimer& m_timer;
};









#endif //SHIELD_TestManager_H