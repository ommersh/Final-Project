#ifndef SHIELD_TestManager_H    // Check if the symbol SHIELD_TestManager_H is not defined
#define SHIELD_TestManager_H    // Define the symbol SHIELD_TestManager_H

#include "ITimer.h"
#include "CommonStructures.h"
#include "TcaCalculation.h"

#include "CATCH.h"
#include "IRootsFindAlg.h"
#include "CompanionMatrixRootsFinder.h"

#include "ANCAS.h"

/// <summary>
/// Manager for running test on the TCA algorithms, giver a set of test parameters and test data, 
/// Initialize a test run of the given algorithm with the given data,
/// Test the run time and return the results.
/// </summary>
class TestManager
{
public:
	TestManager(ITimer* timer);
	virtual ~TestManager();
	/// <summary>
	/// Initialize a test run of the given algorithm with the given data,
	/// Test the run time and return the results.
	/// </summary>
	/// <param name="params">The test parameters</param>
	/// <param name="pointsData">The test data</param>
	/// <returns>The test results</returns>
	virtual TestResults::TestResult runTest(TestRecipe params, TcaCalculation::sPointData* pointsData);
protected:
	ITimer* m_timer;
};








#endif //SHIELD_TestManager_H