#include "LocalFileCommChannelFacade.h"




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
		}
		else
		{
			MessageHeader header;
			//create header for ancas
			header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
			header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
			//copy the header to the buffer
			memcpy(buffer, reinterpret_cast<unsigned char*>(& header), sizeof(MessageHeader));
			*size = sizeof(MessageHeader);
			
			//create the test parameters
			m_params.degree = 15;
			m_params.numberOfPopints = m_fileData.size;
			m_params.testedAlgorithm = TestParameters::Algorithm::ANCAS;
			m_params.catchRootsAlg = TestParameters::CatchRootsAlg::EigenCompanionMatrix;
			//copy the test parameters to the buffer
			memcpy(buffer + *size , reinterpret_cast<unsigned char*>(&m_params), sizeof(TestParameters::TestRecipe));
			*size += sizeof(TestParameters::TestRecipe);

			m_sizeToCompy = m_fileData.size * sizeof(TcaCalculation::sPointData);

			if (m_sizeToCompy < (maxSize - *size))
			{
				//if the buffer is big enough to copy all the data in one go
				//copy the data and go to the next state(catch)
				memcpy(buffer + *size, reinterpret_cast<unsigned char*>(m_fileData.data), m_sizeToCompy);
				*size += m_sizeToCompy;
				m_state = StateWaitForAncasEnd;

			}
			else {
				//fill the buffer with data and go to the next state
				memcpy(buffer + *size, reinterpret_cast<unsigned char*>(m_fileData.data), (maxSize - *size));
				m_offset = (maxSize - *size);
				m_sizeToCompy -= (maxSize - *size);
				*size = maxSize;
				m_state = StateSendAncasData;
			}
			returnValue = true;
		}

		break;

	case StateSendAncasData:
		if (m_sizeToCompy < maxSize)
		{
			memcpy(buffer, reinterpret_cast<unsigned char*>(m_fileData.data) + m_offset, m_sizeToCompy);
			*size = m_sizeToCompy;
			m_state = StateWaitForAncasEnd;

		}
		else {
			memcpy(buffer, reinterpret_cast<unsigned char*>(m_fileData.data) + m_offset, maxSize);
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
		}
		else
		{
			MessageHeader header;
			header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
			header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
			memcpy(buffer, reinterpret_cast<unsigned char*>(&header), sizeof(MessageHeader));
			*size = sizeof(MessageHeader);

			m_params.degree = 15;
			m_params.numberOfPopints = m_fileData.size;
			m_params.testedAlgorithm = TestParameters::Algorithm::CATCH;
			//if (switchCounter++ % 2 == 0)
			//{
				m_params.catchRootsAlg = TestParameters::CatchRootsAlg::EigenCompanionMatrix;
			//}
			/*else
			{
				m_params.catchRootsAlg = TestParameters::CatchRootsAlg::ArmadilloCompanionMatrix;
			}*/
			memcpy(buffer + *size, reinterpret_cast<unsigned char*>(&m_params), sizeof(TestParameters::TestRecipe));
			*size += sizeof(TestParameters::TestRecipe);

			m_sizeToCompy = m_fileData.size * sizeof(TcaCalculation::sPointData);

			if (m_sizeToCompy < (maxSize - *size))
			{
				memcpy(buffer, reinterpret_cast<unsigned char*>(m_fileData.data), m_sizeToCompy);
				*size += m_sizeToCompy;
				m_state = StateWaitForCatchEnd;

			}
			else {
				memcpy(buffer + *size, reinterpret_cast<unsigned char*>(m_fileData.data), (maxSize - *size));
				m_offset = (maxSize - *size);
				m_sizeToCompy -= (maxSize - *size);
				*size = maxSize;
				m_state = StateSendCatchData;
			}
			returnValue = true;
		}

		break;

	case StateSendCatchData:
		if (m_sizeToCompy < maxSize)
		{
			memcpy(buffer, reinterpret_cast<unsigned char*>(m_fileData.data) + m_offset, m_sizeToCompy);
			*size = m_sizeToCompy;
			m_state = StateWaitForCatchEnd;

		}
		else {
			memcpy(buffer, reinterpret_cast<unsigned char*>(m_fileData.data) + m_offset, maxSize);
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
	switch (m_params.testedAlgorithm)
	{
	case TestParameters::Algorithm::CATCH:
	{
		switch (m_params.catchRootsAlg)
		{
		case TestParameters::CatchRootsAlg::EigenCompanionMatrix:
			printResult("CATCH_Eigen", m_params.degree, "LEMUR2_COSMOS", testResults.tca.numberOfPoints, testResults.runTimeMicro, testResults.tca);
			break;
		case TestParameters::CatchRootsAlg::ArmadilloCompanionMatrix:
			printResult("CATCH_Armadillo", m_params.degree, "LEMUR2_COSMOS", testResults.tca.numberOfPoints, testResults.runTimeMicro, testResults.tca);
			break;
		default:
			break;
		}
	}
		break;
	case TestParameters::Algorithm::ANCAS:
		printResult("ANCAS", 3, "LEMUR2_COSMOS", testResults.tca.numberOfPoints, testResults.runTimeMicro, testResults.tca);
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
	m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_CONST.csv");
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
	m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_GAUSS.csv");
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
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: printResult
//		
//	Description: Print the results in a table:
//		testName		|algName        |degree         |numberOfPoints     |runTime(sec)        |runTime(microSec)   |TCA distance   |TCA time
//		--------------- |---------------|---------------|-------------------|--------------------|--------------------|---------------|----------
//		LEMUR2_COSMOS	| ANCAS			| 3				| 6451				| 0.000669			 | 669				  | 1.17159		  | 177096
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::printResult(string algName, int degree, string testName, int numberOfPoints, double runTime, TcaCalculation::TCA tca)
{
	std::cout << std::left << std::setw(15) << testName
		<< "|" << std::left << std::setw(15) << algName
		<< "|" << std::left << std::setw(15) << degree
		<< "|" << std::left << std::setw(20) << numberOfPoints
		<< "|" << std::left << std::setw(20) << runTime / 1000000
		<< "|" << std::left << std::setw(20) << runTime
		<< "|" << std::left << std::setw(15) << tca.distance
		<< "|" << std::left << std::setw(15) << tca.time << std::endl;

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: startPrint
//		
//	Description: Print the start of the table:
//		testName		|algName        |degree         |numberOfPoints     |runTime(sec)        |runTime(microSec)   |TCA distance   |TCA time
//		--------------- |---------------|---------------|-------------------|--------------------|--------------------|---------------|----------
//		LEMUR2_COSMOS	| ANCAS			| 3				| 6451				| 0.000669			 | 669				  | 1.17159		  | 177096
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void LocalFileCommChannelFacade::startPrint()
{
	std::cout << std::left << std::setw(15) << "testName"
		<< "|" << std::left << std::setw(15) << "algName"
		<< "|" << std::left << std::setw(15) << "degree"
		<< "|" << std::left << std::setw(20) << "numberOfPoints"
		<< "|" << std::left << std::setw(20) << "runTime(sec)"
		<< "|" << std::left << std::setw(20) << "runTime(microSec)"
		<< "|" << std::left << std::setw(15) << "TCA distance"
		<< "|" << std::left << std::setw(15) << "TCA time" << std::endl;


	std::cout << std::left << std::setw(15) << std::setfill('-') << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << "" << std::setfill(' ') << std::endl;

}
