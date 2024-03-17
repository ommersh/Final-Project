#include "LocalFileCommChannelFacade.h"
#include <string.h>




//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getNextMessage
//		
//	Description:
//				Implement a state machine for getting the data of CATCH\ANCAS, creating a fake
//				message and returning it as if we received a message
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
bool LocalFileCommChannelFacade::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size)
{
	bool returnValue = false;
	static unsigned char switchCounter = 0;
	//running the state machine
	switch (m_state)
	{
	case StateStart:
		//reset the offset and start immediately
		m_offset = 0;
		m_state = StateGetAncasData;

	case StateGetAncasData:
		//Get the data from the file
		getAncasData();
		//if we failed to get the data reset the state machine
		if (m_fileData.size == -1)
		{
			std::cout << "Failed to read ANCAS data file...\n";
			m_state = StateStart;
			break;
		}
		else
		{
			MessageHeader header;
			m_params = { 0 };

			//create header for ancas
			header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
			header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
			//copy the header to the buffer


			//create the test parameters
			m_params.degree = 15;
			m_params.numberOfPopints = m_fileData.size;
			m_params.testedAlgorithm = TestParameters::Algorithm::ANCAS;
			m_params.catchRootsAlg = TestParameters::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
			// Safe function available on Windows
			strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "LEMUR2_COSMOS");
#else
			// Standard C function, less safe but portable
			strncpy(m_params.testName, "LEMUR2_COSMOS", MAX_TEST_NAME_SIZE);
			m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
			m_params.testID = testID++;
			m_params.numberOfRuns = 100;
			
			m_sizeToCompy = sizeof(MessageHeader) + sizeof(TestParameters::TestRecipe) + m_fileData.size * sizeof(TcaCalculation::sPointData);
			m_messageBuffer = new unsigned char[m_sizeToCompy];
			if (nullptr == m_messageBuffer)
			{
				returnValue = false;
				break;
			}
			int offset = 0;
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&header), sizeof(MessageHeader));
			offset += sizeof(MessageHeader);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&m_params), sizeof(TestParameters::TestRecipe));
			offset += sizeof(TestParameters::TestRecipe);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(m_fileData.data), m_fileData.size * sizeof(TcaCalculation::sPointData));
			m_offset = 0;
		}
	case StateSendAncasData:
		if (m_sizeToCompy < maxSize)
		{
			memcpy(buffer, m_messageBuffer + m_offset, m_sizeToCompy);
			*size = m_sizeToCompy;
			m_state = StateWaitForAncasEnd;

		}
		else {
			memcpy(buffer, m_messageBuffer + m_offset, maxSize);
			m_sizeToCompy -= (maxSize);
			*size = maxSize;
			m_offset += maxSize;
			m_state = StateSendAncasData;
		}
		returnValue = true;
		break;

	case StateWaitForAncasEnd:
		std::cout << "LocalFileCommChannelFacade::StateWaitForAncasEnd\n";
		break;

	case StateGetCatchData:
		getCatchData();
		if (m_fileData.size == -1)
		{
			std::cout << "Failed to read CATCH data file...\n";
			m_state = StateStart;
			break;
		}
		else
		{
			MessageHeader header;
			m_params = { 0 };

			header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
			header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);

			m_params.degree = 15;
			m_params.numberOfPopints = m_fileData.size;
			m_params.testedAlgorithm = TestParameters::Algorithm::CATCH;
#ifdef _WIN32
			// Safe function available on Windows
			strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "LEMUR2_COSMOS");
#else
			// Standard C function, less safe but portable
			strncpy(m_params.testName, "LEMUR2_COSMOS", MAX_TEST_NAME_SIZE);
			m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
			m_params.testID = testID++;
			m_params.numberOfRuns = 100;
			//if (switchCounter++ % 2 == 0)
			//{
			m_params.catchRootsAlg = TestParameters::CatchRootsAlg::EigenCompanionMatrix;
			//}
			/*else
			{
				m_params.catchRootsAlg = TestParameters::CatchRootsAlg::ArmadilloCompanionMatrix;
			}*/

			m_sizeToCompy = sizeof(MessageHeader) + sizeof(TestParameters::TestRecipe) + m_fileData.size * sizeof(TcaCalculation::sPointData);
			m_messageBuffer = new unsigned char[m_sizeToCompy];
			if (nullptr == m_messageBuffer)
			{
				returnValue = false;
				break;
			}
			int offset = 0;
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&header), sizeof(MessageHeader));
			offset += sizeof(MessageHeader);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&m_params), sizeof(TestParameters::TestRecipe));
			offset += sizeof(TestParameters::TestRecipe);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(m_fileData.data), m_fileData.size * sizeof(TcaCalculation::sPointData));
			m_offset = 0;


		}

	case StateSendCatchData:
		if (m_sizeToCompy < maxSize)
		{
			memcpy(buffer, m_messageBuffer + m_offset, m_sizeToCompy);
			*size = m_sizeToCompy;
			m_state = StateWaitForCatchEnd;

		}
		else {
			memcpy(buffer, m_messageBuffer + m_offset, maxSize);
			m_sizeToCompy -= (maxSize);
			*size = maxSize;
			m_offset += maxSize;
			m_state = StateSendCatchData;
		}
		returnValue = true;
		break;
	case StateWaitForCatchEnd:
		std::cout << "LocalFileCommChannelFacade::StateWaitForCatchEnd\n";

		break;
	default:
		break;
	}

	return returnValue;
}


#include <iomanip>

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: sendMessage
//		
//	Description:
//				Print the results based on the last parameters we "sent"
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::sendMessage(unsigned char* buffer, unsigned int size)
{

	MessagesDefinitions::MessageHeader header;
	memcpy(reinterpret_cast<unsigned char*>(&header) , buffer , sizeof(MessagesDefinitions::MessageHeader));
	TestResults::TestResult testResults;
	memcpy(reinterpret_cast<unsigned char*>(&testResults), buffer + sizeof(MessagesDefinitions::MessageHeader),  sizeof(TestResults::TestResult));
	startPrint();
	switch (testResults.testedAlgorithm)
	{
	case TestParameters::Algorithm::CATCH:
	{
		switch (testResults.catchRootsAlg)
		{
		case TestParameters::CatchRootsAlg::EigenCompanionMatrix:
			printResult("CATCH_Eigen", testResults);
			break;
		case TestParameters::CatchRootsAlg::ArmadilloCompanionMatrix:
			printResult("CATCH_Armadillo", testResults);
			break;
		default:
			break;
		}
	}
		break;
	case TestParameters::Algorithm::ANCAS:
		printResult("ANCAS", testResults);
		break;
	};
	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name:getAncasData
//		
//	Description: Read the data for ANCAS from a file
// 
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::getAncasData()
{
	FileReader fr;
	m_fileData = fr.readDataFromFile("LEMUR2_COSMOS_CONST.csv");
	if (m_fileData.size == -1)
	{
		m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_CONST.csv");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getCatchData
//		
//	Description: Read the data for CATCH from a file
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::getCatchData()
{
	FileReader fr;
	m_fileData = fr.readDataFromFile("LEMUR2_COSMOS_GAUSS.csv");

	if (m_fileData.size == -1)
	{
		m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_GAUSS.csv");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: reset
//		
//	Description: Reset between messages
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::reset()
{
	switch (m_state)
	{
	case StateGetAncasData:
	case StateSendAncasData:
	case StateWaitForAncasEnd:
		m_offset = 0;
		m_state = StateGetCatchData;
		break;
	case StateGetCatchData:
	case StateSendCatchData:
	case StateWaitForCatchEnd:
		m_offset = 0;
		m_state = StateStart;
		break;
	case StateStart:
	default:
		std::cout << "LocalFileCommChannelFacade::reset ERROR\n";

		break;
	}
	if (nullptr != m_fileData.data)
	{
		delete[] m_fileData.data;
	}
	if (nullptr != m_messageBuffer)
	{
		delete[] m_messageBuffer;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: printResult
//		
//	Description: Print the results in a table:
//		Test Name      |Test ID        |Alg Name       |Degree         |NumberOfPoints      |RunTime(sec)        |RunTime(microSec)   |NumberOfRuns   |avgTimeMicro   |minTimeMicro   |TCA distance		 |TCA time
//		---------------|---------------|---------------|---------------|--------------------|--------------------|--------------------|---------------|---------------|---------------|------------------|---------------
//		LEMUR2_COSMOS  | 0			   | ANCAS		   | 15			   | 6451				| 0.000676			 | 676				  | 100			  | 645.49		  | 627			  | 1.17159132160904 | 177095.670099459
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::printResult(string algName, TestResults::TestResult results)
{
		std::cout << std::setprecision(15) <<
		std::left << std::setw(15) << results.testName
		<< "|" << std::left << std::setw(15) << results.testID
		<< "|" << std::left << std::setw(15) << algName
		<< "|" << std::left << std::setw(15) << results.degree
		<< "|" << std::left << std::setw(20) << results.tca.numberOfPoints
		<< "|" << std::left << std::setw(20) << results.runTimeMicro / 1000000
		<< "|" << std::left << std::setw(20) << results.runTimeMicro

		<< "|" << std::left << std::setw(15) << results.numberOfRuns
		<< "|" << std::left << std::setw(15) << results.avgTimeMicro
		<< "|" << std::left << std::setw(15) << results.minTimeMicro


		<< "|" << std::left << std::setw(20) << results.tca.distance
		<< "|" << std::left << std::setw(20) << results.tca.time << std::endl;

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: startPrint
//		
//	Description: Print the start of the table:
//		Test Name      |Test ID        |Alg Name       |Degree         |NumberOfPoints      |RunTime(sec)        |RunTime(microSec)   |NumberOfRuns   |avgTimeMicro   |minTimeMicro   |TCA distance		 |TCA time
//		---------------|---------------|---------------|---------------|--------------------|--------------------|--------------------|---------------|---------------|---------------|------------------|---------------
//		LEMUR2_COSMOS  | 0			   | ANCAS		   | 15			   | 6451				| 0.000676			 | 676				  | 100			  | 645.49		  | 627			  | 1.17159132160904 | 177095.670099459
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::startPrint()
{
	std::cout << std::left << std::setw(15) << "Test Name"
		<< "|" << std::left << std::setw(15) << "Test ID"
		<< "|" << std::left << std::setw(15) << "Alg Name"
		<< "|" << std::left << std::setw(15) << "Degree"
		<< "|" << std::left << std::setw(20) << "NumberOfPoints"
		<< "|" << std::left << std::setw(20) << "RunTime(sec)"
		<< "|" << std::left << std::setw(20) << "RunTime(microSec)"

		<< "|" << std::left << std::setw(15) << "NumberOfRuns"
		<< "|" << std::left << std::setw(15) << "avgTimeMicro"
		<< "|" << std::left << std::setw(15) << "minTimeMicro"

		<< "|" << std::left << std::setw(20) << "TCA distance"
		<< "|" << std::left << std::setw(20) << "TCA time" << std::endl;


	std::cout << std::left << std::setw(15) << std::setfill('-') << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""

		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << ""

		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""

		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << "" << std::setfill(' ') << std::endl;

}
