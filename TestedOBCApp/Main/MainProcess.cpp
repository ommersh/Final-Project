#include "MainProcess.h"








MainProcess::MainProcess(ITimer& timer) : m_testManager(timer)
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

		//wait for the test to end

		//manage the test results

		//send the test results back

		//sleep some time
	}
}