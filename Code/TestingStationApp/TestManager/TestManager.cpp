#include "TestManager.h"
#include <thread>
#include <iostream>
#include "Lab.h"
#include "EventLogger.h"

TestManager::TestManager() :
	m_commManger(nullptr),
	m_resultsManager(nullptr)
{
	m_keepRunnig = true;
	m_state = eWaitingForTheNextTest;
	m_startTime = 0;
}

//Initialize the class
void TestManager::init(ResultManager& resultsManager, CommManager& commManger)
{
	m_resultsManager = &resultsManager;
	m_commManger = &commManger;
	m_keepRunnig = true;
	m_state = eWaitingForTheNextTest;
	m_thisThread = std::thread(&TestManager::RunTestManagerProcess, this);

}

//synchoronized access to the queue!
void TestManager::PlaceTestInQueue(TestRecipe recipe, TcaCalculation::sPointData* pointsDataArray, int arraySize)
{
	TestInQueue testInQueu = { 0 };
	testInQueu.recipe = recipe;
	testInQueu.pointsDataArray = pointsDataArray;
	testInQueu.arraySize = arraySize;
	//Add the new test to the queue
	m_waitingTestQueue.enqueue(testInQueu);
}

//Run forever, send test recipes and collect results
void TestManager::RunTestManagerProcess()
{
	TestInQueue nextTest = { 0 };
	std::string logString = "";
	while (m_keepRunnig)
	{
		switch (m_state)
		{
		case eWaitingForTheNextTest:
			//start by getting the next test(if there is one)
			if (m_waitingTestQueue.isEmpty() == false)
			{
				if (m_waitingTestQueue.dequeue(nextTest) == true)
				{
					//send the data, check if the data sent successfully
					if (m_commManger->SendMessage(nextTest.recipe, nextTest.pointsDataArray) == true)
					{

						m_state = eWaitingForTestResults;
						m_startTime = getCurrentTimeInMicroseconds();
						//Update the test state
						Lab::GetInstance().updateTestStatus(TestStatus::InProgress, nextTest.recipe.testID);
						logString = "Test Request Sent Successfully For Test - " + std::to_string(nextTest.recipe.testID);
						EventLogger::getInstance().log(logString, "TestManager");

					}
					else
					{
						//we got the test data but failed to send! handle the error!
						logString = "Failed To Send The Following Test - " + std::to_string(nextTest.recipe.testID) + " Returning The Test To Queue";
						EventLogger::getInstance().log(logString, "TestManager");
						m_waitingTestQueue.returnToTopOFQueue(nextTest);
					}
				}
			}
			else {
				//Continue to wait for the incoming test
				//Try sleeping for a little while! 
				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MILLISEC));
			}
			break;
		case eWaitingForTestResults:
			//If a message received
			if (m_commManger->GetNextMessage() == true)
			{
				TestResults::TestResult results = m_commManger->GetLastReceivedTestResult();
				//Confirm we received the correct results
				if (results.testID == nextTest.recipe.testID)
				{
					m_resultsManager->UpdateTestResult(results);

					logString = "Test Results Arrived For Test - " + std::to_string(nextTest.recipe.testID);
					EventLogger::getInstance().log(logString, "TestManager");

					//delete the data
					if (nextTest.pointsDataArray != nullptr)
					{
						delete[] nextTest.pointsDataArray;
					}
					//reset the nextTest object
					nextTest = { 0 };
					m_state = eWaitingForTheNextTest;
				}
				else
				{
					logString = "Error!! Received Results For A Different Test! Expected Test ID - " + std::to_string(nextTest.recipe.testID)
						+ " Received Test ID - " + std::to_string(results.testID);
					EventLogger::getInstance().log(logString, "TestManager");
				}
			}
			//Check for timeout
			else if ((getCurrentTimeInMicroseconds() - m_startTime) >= TIME_OUT_MICROSEC)
			{
				//Timeout, No results arrived, handle the error!
				logString = "Timeout! No Results Arrived For The Following Test - " + std::to_string(nextTest.recipe.testID) + " Returning The Test To Queue";
				EventLogger::getInstance().log(logString, "TestManager");

				m_waitingTestQueue.returnToTopOFQueue(nextTest);
				m_state = eWaitingForTheNextTest;
			}
			else {
				//Continue to wait for the results
				//Try sleeping for a little while! 
				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MILLISEC));
			}
			break;
		default:
			break;
		}
		
	}
}

//Check if the communication seems to be active(synchoronized)
bool TestManager::CheckCommunication()
{
	return true;
}

void TestManager::startTestManagerProcess()
{
	m_keepRunnig = true;
}

void TestManager::stopTestManagerProcess()
{
	m_keepRunnig = false;
}

unsigned long int TestManager::getCurrentTimeInMicroseconds() {
	// Get the current time point using the high resolution clock
	auto now = std::chrono::high_resolution_clock::now();

	// Convert the time point to microseconds since the epoch
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

	return microseconds;
}
