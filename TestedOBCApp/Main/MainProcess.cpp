#include "MainProcess.h"








MainProcess::MainProcess(TestManager* testManager) : m_testManager(testManager)
{

}

MainProcess::~MainProcess()
{

}

void MainProcess::process()
{
	//Running forever
	while (true)
	{
		//Check for a new data set and test setting from the communication channel


		//run the test
		//m_testManager->runTest()

		//wait for the test to end

		//manage the test results

		//send the test results back

		//sleep some time
	}
}