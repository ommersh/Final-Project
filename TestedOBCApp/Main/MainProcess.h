#ifndef SHIELD_MainProcess_H    // Check if the symbol SHIELD_MainProcess_H is not defined
#define SHIELD_MainProcess_H    // Define the symbol SHIELD_MainProcess_H

#include "TestManager.h"
#include "ITimer.h"





class MainProcess
{
public:
	MainProcess(ITimer& timer);
	~MainProcess();
	virtual void process();
private:
	TestManager m_testManager;

};









#endif //SHIELD_MainProcess_H