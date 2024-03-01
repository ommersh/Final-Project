#ifndef SHIELD_CommManager_H    // Check if the symbol SHIELD_CommManager_H is not defined
#define SHIELD_CommManager_H    // Define the symbol SHIELD_CommManager_H

#include "TCA_Calculation.h"
#include "TestDefinitions.h"
#include "ICommChannel.h"

class CommManager
{
public:
	CommManager(ICommChannel& commChannel);
	virtual ~CommManager();
	virtual void init();
	virtual bool getTheNextTest();
	virtual TestParameters::TestParams getTheTestParameters();
	virtual TcaCalculation::sPointData* getTheTestData();

	/// <summary>
	/// Call the function when the test end to free all the memory if nececary
	/// </summary>
	virtual void endTest();
	virtual void sendTestResults(TestResults::TestResult testResults);
protected:
	ICommChannel& m_commChannel;
	TestParameters::TestParams m_testParameters;
	TcaCalculation::sPointData* m_pointsData;
};
#endif //SHIELD_CommManager_H