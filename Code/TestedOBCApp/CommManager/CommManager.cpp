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
	resetParser();
	unsigned short opCode = MessagesDefinitions::TestRequestMessageOpcode;
	initParser(reinterpret_cast<unsigned char*>(&opCode));
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
	static const unsigned int BUFFER_SIZE = MAX_MESSAGE_SIZE;
	unsigned char buffer[BUFFER_SIZE];
	unsigned int size = 0;
	unsigned int offset = 0;
	bool messageReceived = false;

	//read everything from the channel
	while (m_commChannel.getNextMessage(buffer, BUFFER_SIZE, &size))
	{
		//if we found a full message
		if (true == parseBuffer(buffer, size))
		{
			MessagesDefinitions::MessageHeader header;
			offset = 0;
			memcpy(reinterpret_cast<unsigned char*>(&header), m_messageBuffer + offset, MessagesDefinitions::MESSAGE_HEADER_SIZE);
			offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
			//get the test parameters
			memcpy(reinterpret_cast<unsigned char*>(&m_testParameters), m_messageBuffer + offset, sizeof(TestRecipe));
			offset += sizeof(TestRecipe);
			std::cout << "Received Test " << m_testParameters.testID << std::endl;

			//get the data
			int numberOfPoints = m_messageHeader.dataSize / sizeof(TcaCalculation::sPointData);
			m_pointsData = new TcaCalculation::sPointData[numberOfPoints];

			memcpy(reinterpret_cast<unsigned char*>(m_pointsData), m_messageBuffer + offset, m_messageHeader.dataSize);

			resetParser();
			messageReceived = true;
			break;
		}
	}
	resetParser();
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
TestRecipe CommManager::getTheTestParameters()
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
	resetParser();
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


void CommManager::initParser(unsigned char opcode[MessagesDefinitions::OPCODE_SIZE])
{
	for (int i = 0; i < MessagesDefinitions::OPCODE_SIZE; i++)
	{
		m_opCode[i] = opcode[i];
	}
	m_messageBuffer = nullptr;
	resetParser();
}


bool CommManager::parseBuffer(unsigned char* buffer, int size)
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
					std::cout << "Found Opcode " << std::endl;
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
					std::cout << "Failed to created Message Buffer!!!" << std::endl;
					resetParser();
					return false;
				}
				memcpy(m_messageBuffer, reinterpret_cast<unsigned char*>(&m_messageHeader), MessagesDefinitions::MESSAGE_HEADER_SIZE);
				m_messageBufferIndex = MessagesDefinitions::MESSAGE_HEADER_SIZE;
				m_parserState = CommManager::CollectingData;
				std::cout << "Found header " << std::endl;
			}
			break;
		case CommManager::CollectingData:
			if (m_messageBufferIndex < m_messageSize)
			{
				m_messageBuffer[m_messageBufferIndex++] = *buffer;
			}
			if (m_messageBufferIndex >= m_messageSize)
			{
				m_parserState = CommManager::Done;
				fullMessageFound = true;
				std::cout << "Found full message " << std::endl;

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

void CommManager::resetParser()
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

