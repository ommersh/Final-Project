#ifndef SHIELD_CommManager_H    // Check if the symbol SHIELD_CommManager_H is not defined
#define SHIELD_CommManager_H    // Define the symbol SHIELD_CommManager_H

#include "CommonStructures.h"
#include "ICommChannel.h"

/// <summary>
/// Manager for the communication between the TestingStationApp and the TestedOBCApp.
/// Using a ICommChannel receive raw data, parse it into the relevant test data and send the results at the end.
/// </summary>
class CommManager
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="commChannel">The communication channel to the TestingStationApp</param>
	CommManager(ICommChannel& commChannel);
	virtual ~CommManager();
	/// <summary>
	/// Initialize the structs and communication channel if necessary
	/// </summary>
	virtual void init();
	/// <summary>
	/// Get the next available test command from the communication channel(if exist)
	/// </summary>
	/// <returns>true if a messgae received</returns>
	virtual bool getTheNextTest();
	/// <summary>
	/// Get the test parameters from the last message received
	/// </summary>
	/// <returns></returns>
	virtual TestParameters::TestRecipe getTheTestParameters();
	/// <summary>
	///Get the test data from the last message received
	/// </summary>
	/// <returns></returns>
	virtual TcaCalculation::sPointData* getTheTestData();
	/// <summary>
	/// Call the function when the test end to free all the memory if necessary
	/// </summary>
	virtual void endTest();
	/// <summary>
	/// Send the results back to the TestingStationApp
	/// </summary>
	/// <param name="testResults"></param>
	virtual void sendTestResults(TestResults::TestResult testResults);
protected:
	ICommChannel& m_commChannel;
	TestParameters::TestRecipe m_testParameters;
	TcaCalculation::sPointData* m_pointsData;
};
#endif //SHIELD_CommManager_H