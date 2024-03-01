#ifndef SHIELD_MainProcess_H    // Check if the symbol SHIELD_MainProcess_H is not defined
#define SHIELD_MainProcess_H    // Define the symbol SHIELD_MainProcess_H

#include "TestManager.h"
#include "CommManager.h"

#include "ITimer.h"





class MainProcess
{
public:
	MainProcess(TestManager* testManager, CommManager* commManager);
	virtual ~MainProcess();
	virtual void process();
private:
	TestManager* m_testManager;
	CommManager* m_commManager;
};









#endif //SHIELD_MainProcess_H