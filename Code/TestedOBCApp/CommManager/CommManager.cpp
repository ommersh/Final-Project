#include "CommManager.h"
#include <string.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: CommManager
//		
//	Description: Constructor
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
CommManager::CommManager(ICommChannel& commChannel) : m_commChannel(commChannel)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: CommManager
//		
//	Description: Constructor
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
CommManager::~CommManager()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: init
//		
//	Description: Initialize the structs and communication channel if necessary
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void CommManager::init()
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getTheNextTest
//		
//	Description: Get the next available test command from the communication channel(if exist)
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
bool CommManager::getTheNextTest()
{
	unsigned char buffer[1024];
	unsigned int offset = 0;
	unsigned int size = 0;
	unsigned int remainingDataSize;
	bool messageReceived = false;

	//check for a message
	if (m_commChannel.getNextMessage(buffer, 1024, &size))
	{
		//get the message header
		MessagesDefinitions::MessageHeader header;
		memcpy(&header, buffer, sizeof(MessagesDefinitions::MessageHeader));
		offset += sizeof(MessagesDefinitions::MessageHeader);
		//check if its a test request message
		if (header.opcode == MessagesDefinitions::TestRequestMessageOpcode)
		{
			//get the size of the data 
			//unsigned int dataSize = header.dataSize / sizeof(TcaCalculation::sPointData);

			//get the test parameters
			memcpy(reinterpret_cast<unsigned char*>(& m_testParameters), buffer + offset, sizeof(TestParameters::TestRecipe));
			offset += sizeof(TestParameters::TestRecipe);

			//get the data
			m_pointsData = new TcaCalculation::sPointData[header.dataSize / sizeof(TcaCalculation::sPointData)];

			memcpy(reinterpret_cast<unsigned char*>(m_pointsData), buffer + offset, size - offset);
			remainingDataSize = header.dataSize - (size - offset);
			offset = size - offset;
			//get the rest of the data
			while (remainingDataSize > 0 && m_commChannel.getNextMessage(buffer, 1024, &size))
			{
				if (size > remainingDataSize)
				{
					size = remainingDataSize;
				}

				memcpy(reinterpret_cast<unsigned char*>(m_pointsData) + offset, buffer, size);
				offset += size;
				remainingDataSize = remainingDataSize - size;
			}

			//if we got all the data we got the message!
			if (remainingDataSize == 0)
			{
				messageReceived = true;
			}
			else
			{
				delete[] m_pointsData;
			}
		}
	}
	return messageReceived;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getTheTestParameters
//		
//	Description: Get the test parameters from the last message received
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
TestParameters::TestRecipe CommManager::getTheTestParameters()
{
	return m_testParameters;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getTheTestData
//		
//	Description: Get the test data from the last message received
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
TcaCalculation::sPointData* CommManager::getTheTestData()
{
	return m_pointsData;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: endTest
//		
//	Description:  Call the function when the test end to free all the memory if necessary
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void CommManager::endTest()
{
	if (nullptr != m_pointsData)
	{
		delete[] m_pointsData;
	}
	m_commChannel.reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: sendTestResults
//		
//	Description: Send the results back to the TestingStationApp
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void CommManager::sendTestResults(TestResults::TestResult testResults)
{
	unsigned char buffer[sizeof(MessagesDefinitions::MessageHeader) + sizeof(TestResults::TestResult)];
	unsigned int size = sizeof(MessagesDefinitions::MessageHeader) + sizeof(TestResults::TestResult);
	MessagesDefinitions::MessageHeader header;
	header.opcode = MessagesDefinitions::TestResultsMessageOpcode;
	header.dataSize = sizeof(TestResults::TestResult);
	memcpy(buffer, reinterpret_cast<unsigned char*>(&header), sizeof(MessagesDefinitions::MessageHeader));
	memcpy(buffer + sizeof(MessagesDefinitions::MessageHeader), reinterpret_cast<unsigned char*>(&testResults), sizeof(TestResults::TestResult));

	m_commChannel.sendMessage(buffer, size);
}