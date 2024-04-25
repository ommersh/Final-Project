#include "MainProcess.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "EventLogger.h"


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



void MainProcess::Process()
{
	std::cout << "Staring MainProcess " << std::endl;
	EventLogger::getInstance().log("Staring MainProcess", "MainProcess");
	std::string logString = "";
	//Running forever
	while (true == g_keepRunning)
	{
		//Check for a new data set and test setting from the communication channel
		if (m_commManager->GetTheNextTest())
		{
			//collect the data
			TestRecipe params = m_commManager->GetTheTestRecipe();
			TcaCalculation::sPointData* data = m_commManager->GetTheTestData();

			logString = "Test Received. Staring test - " + std::to_string(params.testID);
			EventLogger::getInstance().log(logString, "MainProcess");
			std::cout << logString << std::endl;

			//run the test
			//m_testManager->runTest()
			TestResults::TestResult testResults = m_testManager->runTest(params, data);

			//wait for the test to end

			//manage the test results
			resultsLogger->log(testResults, params.TOLd, params.TOLt);

			//send the test results back
			m_commManager->SendTestResults(testResults);

			logString = "Test " + std::to_string(params.testID) + " Completed";
			EventLogger::getInstance().log(logString, "MainProcess");
			std::cout << logString << std::endl;

			//end the test, reset everything and free any memory
			m_commManager->EndTest();
		}
		//sleep some time
		//std::cout << "Sleeping for 1 seconds...\n";

		// Sleep for 3 seconds
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}

}