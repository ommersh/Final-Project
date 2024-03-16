#ifndef SHIELD_MainProcess_H    // Check if the symbol SHIELD_MainProcess_H is not defined
#define SHIELD_MainProcess_H    // Define the symbol SHIELD_MainProcess_H

#include "TestManager.h"
#include "CommManager.h"

#include "ITimer.h"
#include "ResultsLogger.h"



/// <summary>
/// The main process of the TestedOBCApp
/// Run nonstop, try to get a message, run a test, send the results back and wait for the next messgae.
/// </summary>
class MainProcess
{
public:
	MainProcess(TestManager* testManager, CommManager* commManager, ResultsLogger* logger);
	virtual ~MainProcess();
	/// <summary>
	/// Run nonstop, try to get a message, run a test, send the results back and wait for the next messgae.
	/// </summary>
	virtual void process();
private:
	TestManager* m_testManager;
	CommManager* m_commManager;
	ResultsLogger* resultsLogger;

};









#endif //SHIELD_MainProcess_H