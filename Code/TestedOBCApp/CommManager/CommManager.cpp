#include "CommManager.h"
#include <string.h>
#include <iostream>
#include "Utilities.h"
#include "EventLogger.h"
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
	m_pointsData = nullptr;
	m_messageBuffer = nullptr;

	m_opCodeBufferIndex = 0;
	m_headerBufferIndex = 0;
	m_messageBufferIndex = 0;
	m_messageSize = 0;
	m_parserState = LookingForOpCode;
	m_messageBuffer = nullptr;
	memset(m_opCodeBuffer, 0, MessagesDefinitions::OPCODE_SIZE);
	memset(m_opCode, 0, MessagesDefinitions::OPCODE_SIZE);
	memset(m_headerBuffer, 0, MessagesDefinitions::MESSAGE_HEADER_SIZE);
	m_messageHeader = { 0 };
	m_testParameters = { 0 };

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
void CommManager::Init()
{
	ResetParser();
	unsigned short opCode = MessagesDefinitions::TestRequestMessageOpcode;
	InitParser(reinterpret_cast<unsigned char*>(&opCode));
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getTheNextTest
//		
//	Description: Get the next available test command from the communication channel(if exist)
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
bool CommManager::GetTheNextTest()
{
	static const unsigned int BUFFER_SIZE = MAX_MESSAGE_SIZE;
	unsigned char buffer[BUFFER_SIZE];
	unsigned int size = 0;
	unsigned int offset = 0;
	bool messageReceived = false;

	//read everything from the channel
	while (m_commChannel.getNextMessage(buffer, BUFFER_SIZE, &size))
	{
		//Call the parser on the incoming buffer
		if (true == ParseBuffer(buffer, size))
		{
			//if we found a full message
			offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
			//get the test parameters
			memcpy(reinterpret_cast<unsigned char*>(&m_testParameters), m_messageBuffer + offset, sizeof(TestRecipe));
			offset += sizeof(TestRecipe);
			//get the data
			int numberOfPoints = m_messageHeader.dataSize / sizeof(TcaCalculation::sPointData);
			m_pointsData = new TcaCalculation::sPointData[numberOfPoints];
			memcpy(reinterpret_cast<unsigned char*>(m_pointsData), m_messageBuffer + offset, m_messageHeader.dataSize);

			ResetParser();
			messageReceived = true;
			break;
		}
	}
	ResetParser();
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
TestRecipe CommManager::GetTheTestRecipe()
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
TcaCalculation::sPointData* CommManager::GetTheTestData()
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
void CommManager::EndTest()
{
	if (nullptr != m_pointsData)
	{
		delete[] m_pointsData;
		m_pointsData = nullptr;
	}
	ResetParser();
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
void CommManager::SendTestResults(TestResults::TestResult testResults)
{
	MessagesDefinitions::TestResultsMessage testResultsMessage = { 0 };
	unsigned char* buffer = reinterpret_cast<unsigned char *>(&testResultsMessage);
	unsigned int size = sizeof(MessagesDefinitions::TestResultsMessage);
	testResultsMessage.header.opcode = MessagesDefinitions::TestResultsMessageOpcode;
	testResultsMessage.header.dataSize = sizeof(TestResults::TestResult);
	testResultsMessage.results = testResults;
	//calculate crc
	testResultsMessage.header.crc = CRC32::calculate(buffer + MessagesDefinitions::MESSAGE_HEADER_SIZE, size - MessagesDefinitions::MESSAGE_HEADER_SIZE);
	m_commChannel.sendMessage(buffer, size);
	std::string logString = "Results Message sent. For test - " + std::to_string(testResults.testID);
	EventLogger::getInstance().log(logString, "CommManager::SendTestResults");
}


void CommManager::InitParser(unsigned char opcode[MessagesDefinitions::OPCODE_SIZE])
{
	for (int i = 0; i < MessagesDefinitions::OPCODE_SIZE; i++)
	{
		m_opCode[i] = opcode[i];
	}
	m_messageBuffer = nullptr;
	ResetParser();
}

bool CommManager::ParseBuffer(unsigned char* buffer, int32_t size)
{
	bool fullMessageFound = false;
	while (size > 0)
	{
		switch (m_parserState)
		{
		case CommManager::LookingForOpCode:
			//If its the start of the opcode
			if (*buffer == m_opCode[0])
			{
				m_opCodeBufferIndex = 0;
			}
			//Save the opcode in the buffer
			if (m_opCodeBufferIndex < MessagesDefinitions::OPCODE_SIZE)
			{
				m_opCodeBuffer[m_opCodeBufferIndex++] = *buffer;
			}
			//If we found the full opcode, lets check it
			if (m_opCodeBufferIndex >= MessagesDefinitions::OPCODE_SIZE)
			{
				bool opcodeFound = true;
				for (int i = 0; i < MessagesDefinitions::OPCODE_SIZE; i++)
				{
					if (m_opCodeBuffer[i] != m_opCode[i])
					{
						//It wasnt the opcode, continue looking for the opcode
						opcodeFound = false;
						m_opCodeBufferIndex = 0;
					}
				}
				if (true == opcodeFound)
				{
					//we found the opcode, place it in the header buffer and continue looking for the header
					m_headerBufferIndex = 0;
					for (int i = 0; i < MessagesDefinitions::OPCODE_SIZE; i++)
					{
						m_headerBuffer[m_headerBufferIndex++] = m_opCodeBuffer[i];
					}
					m_headerBufferIndex = MessagesDefinitions::OPCODE_SIZE;
					m_parserState = CommManager::CollectingHeader;
				}
			}
			break;
		case CommManager::CollectingHeader:
			//Save the header in the buffer

			if (m_headerBufferIndex < MessagesDefinitions::MESSAGE_HEADER_SIZE)
			{
				m_headerBuffer[m_headerBufferIndex++] = *buffer;
			}
			//If we found the full header, lets check it
			if (m_headerBufferIndex >= MessagesDefinitions::MESSAGE_HEADER_SIZE)
			{
				memcpy(reinterpret_cast<unsigned char*>(&m_messageHeader), m_headerBuffer, MessagesDefinitions::MESSAGE_HEADER_SIZE);
				m_messageSize = m_messageHeader.dataSize + sizeof(TestRecipe) + MessagesDefinitions::MESSAGE_HEADER_SIZE;

				m_messageBuffer = new unsigned char[m_messageSize];
				if (nullptr == m_messageBuffer)
				{
					//std::cout << "Failed to created Message Buffer!!!" << std::endl;
					EventLogger::getInstance().log("Failed to created Message Buffer!!!", "CommManager::ParseBuffer");

					ResetParser();
					return false;
				}
				memcpy(m_messageBuffer, reinterpret_cast<unsigned char*>(&m_messageHeader), MessagesDefinitions::MESSAGE_HEADER_SIZE);
				m_messageBufferIndex = MessagesDefinitions::MESSAGE_HEADER_SIZE;
				m_parserState = CommManager::CollectingData;
			}
			break;
		case CommManager::CollectingData:
			if (m_messageBufferIndex < m_messageSize)
			{
				m_messageBuffer[m_messageBufferIndex++] = *buffer;
			}
			if (m_messageBufferIndex >= m_messageSize)
			{
				//check for crc errors
				uint32_t crc = CRC32::calculate(m_messageBuffer + MessagesDefinitions::MESSAGE_HEADER_SIZE, m_messageSize - MessagesDefinitions::MESSAGE_HEADER_SIZE);
				if (m_messageHeader.crc == crc)
				{
					m_parserState = CommManager::Done;
					fullMessageFound = true;
				}
				else
				{
					m_parserState = CommManager::LookingForOpCode;
					fullMessageFound = false;
					//std::cout << "CRC Error" << std::endl;
					EventLogger::getInstance().log("CRC Error!!!", "CommManager::ParseBuffer");

				}
				break;
			}
			break;
		case CommManager::Done:
			break;
		default:
			break;
		}
		size--;
		buffer++;
	}
	return fullMessageFound;
}

void CommManager::ResetParser()
{
	m_opCodeBufferIndex = 0;
	m_headerBufferIndex = 0;
	m_messageBufferIndex = 0;
	m_messageSize = 0;
	m_parserState = LookingForOpCode;
	if (nullptr != m_messageBuffer)
	{
		delete[] m_messageBuffer;
		m_messageBuffer = nullptr;
	}
}

