#include "MainProcess.h"




extern bool g_keepRunning;



MainProcess::MainProcess(TestManager* testManager, CommManager* commManager, ResultsLogger* logger) 
	: m_testManager(testManager),
	m_commManager(commManager),
	resultsLogger(logger)
{

}

MainProcess::~MainProcess()
{

}

#include <iostream>
#include <chrono>
#include <thread>

void MainProcess::process()
{
	//Running forever
	while (true == g_keepRunning)
	{
		//Check for a new data set and test setting from the communication channel

		if (m_commManager->getTheNextTest())
		{
			//collect the data
			TestParameters::TestRecipe params = m_commManager->getTheTestParameters();
			TcaCalculation::sPointData* data = m_commManager->getTheTestData();
			//run the test
			//m_testManager->runTest()
			TestResults::TestResult testResults = m_testManager->runTest(params, data);

			//wait for the test to end

			//manage the test results
			resultsLogger->log(testResults, params.TOLd, params.TOLt);

			//send the test results back
			m_commManager->sendTestResults(testResults);

			//end the test, reset everything and free any memory
			m_commManager->endTest();
		}
		//sleep some time
		//std::cout << "Sleeping for 3 seconds...\n";

		// Sleep for 3 seconds
		//std::this_thread::sleep_for(std::chrono::seconds(3));
	}

}