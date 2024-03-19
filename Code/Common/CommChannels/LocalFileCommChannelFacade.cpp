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
		m_state = StateGetTestData;

	case StateGetTestData:
		//Get the data from the file
		switch (switchCounter++ % 3)
		{
		case 0:
			getAncasData();

			break;
		case 1:
			getCatchData();

			break;
		case 2:
			getSboAncasData();

			break;
			}
		//if we failed to get the data reset the state machine
		if (m_fileData.size == -1)
		{
			std::cout << "Failed to get data...\n";
			m_state = StateStart;
			break;
		}
		else
		{
			
			
			m_sizeToCompy = sizeof(MessageHeader) + sizeof(TestParameters::TestRecipe) + m_fileData.size * sizeof(TcaCalculation::sPointData);
			m_messageBuffer = new unsigned char[m_sizeToCompy];
			if (nullptr == m_messageBuffer)
			{
				returnValue = false;
				break;
			}
			int offset = 0;
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&m_header), sizeof(MessageHeader));
			offset += sizeof(MessageHeader);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&m_params), sizeof(TestParameters::TestRecipe));
			offset += sizeof(TestParameters::TestRecipe);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(m_fileData.data), m_fileData.size * sizeof(TcaCalculation::sPointData));
			m_offset = 0;
		}
	case StateSendTestData:
		if (m_sizeToCompy < maxSize)
		{
			memcpy(buffer, m_messageBuffer + m_offset, m_sizeToCompy);
			*size = m_sizeToCompy;
			m_state = StateWaitForTestEnd;

		}
		else {
			memcpy(buffer, m_messageBuffer + m_offset, maxSize);
			m_sizeToCompy -= (maxSize);
			*size = maxSize;
			m_offset += maxSize;
			m_state = StateSendTestData;
		}
		returnValue = true;
		break;

	case StateWaitForTestEnd:
		m_state = StateStart;

		std::cout << "LocalFileCommChannelFacade::StateWaitForTestEnd\n";
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
	case TestParameters::Algorithm::SBO_ANCAS:
		printResult("SBO_ANCAS", testResults);
		break;
	default:
		printResult("NA", testResults);
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
	//FileReader fr;
	/*m_fileData = fr.readDataFromFile("LEMUR2_COSMOS_CONST.csv");
	if (m_fileData.size == -1)
	{
		m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_CONST.csv");
	}
	m_params = { 0 };
	*/
	char Obj1le1[] = "1 57446U 23105S   24079.07379119  .00001728  00000+0  14299-3 0  9991";
	char Obj1le2[] = "2 57446  43.0019 204.3244 0000887 254.7861 105.2883 15.02534958 37324";

	char Obj2le1[] = "1 58755U 23185S   24078.46734562  .00004075  00000+0  31202-3 0  9993";
	char Obj2le2[] = "2 58755  97.6287 146.6053 0015022 228.5263 131.4673 15.02344828 10197";

	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, 14, 16, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min);
	m_fileData.size = m_SimpleDataGeneration.m_numberOfPoints;
	m_fileData.data = m_SimpleDataGeneration.m_pointsDataANCAS;

	//create header for ancas
	m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
	m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
	//copy the header to the buffer


	//create the test parameters
	m_params.catchPolynomialDegree = 3;
	m_params.numberOfPopints = m_fileData.size;
	m_params.testedAlgorithm = TestParameters::Algorithm::ANCAS;
	m_params.catchRootsAlg = TestParameters::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "STARLINK_NANOFF_B");
#else
	// Standard C function, less safe but portable
	strncpy(m_params.testName, "STARLINK_NANOFF_B", MAX_TEST_NAME_SIZE);
	m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	m_params.testID = testID++;
	m_params.numberOfRuns = 100;
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
	/*FileReader fr;
	m_fileData = fr.readDataFromFile("LEMUR2_COSMOS_GAUSS.csv");

	if (m_fileData.size == -1)
	{
		m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_GAUSS.csv");
	}*/

	char Obj1le1[] = "1 57446U 23105S   24079.07379119  .00001728  00000+0  14299-3 0  9991";
	char Obj1le2[] = "2 57446  43.0019 204.3244 0000887 254.7861 105.2883 15.02534958 37324";

	char Obj2le1[] = "1 58755U 23185S   24078.46734562  .00004075  00000+0  31202-3 0  9993";
	char Obj2le2[] = "2 58755  97.6287 146.6053 0015022 228.5263 131.4673 15.02344828 10197";

	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, 14, 16, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min);
	m_fileData.size = m_SimpleDataGeneration.m_numberOfPoints;
	m_fileData.data = m_SimpleDataGeneration.m_pointsDataCATCH;
	
	
	m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
	m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);

	m_params.catchPolynomialDegree = 15;
	m_params.numberOfPopints = m_fileData.size;
	m_params.testedAlgorithm = TestParameters::Algorithm::CATCH;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "STARLINK_NANOFF_B");
#else
	// Standard C function, less safe but portable
	strncpy(m_params.testName, "STARLINK_NANOFF_B", MAX_TEST_NAME_SIZE);
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
void LocalFileCommChannelFacade::getSboAncasData()
{
	//FileReader fr;
	/*m_fileData = fr.readDataFromFile("LEMUR2_COSMOS_CONST.csv");
	if (m_fileData.size == -1)
	{
		m_fileData = fr.readDataFromFile("../../../Implementations/TestApp/LEMUR2_COSMOS_CONST.csv");
	}
	m_params = { 0 };
	*/
	char Obj1le1[] = "1 57446U 23105S   24079.07379119  .00001728  00000+0  14299-3 0  9991";
	char Obj1le2[] = "2 57446  43.0019 204.3244 0000887 254.7861 105.2883 15.02534958 37324";

	char Obj2le1[] = "1 58755U 23185S   24078.46734562  .00004075  00000+0  31202-3 0  9993";
	char Obj2le2[] = "2 58755  97.6287 146.6053 0015022 228.5263 131.4673 15.02344828 10197";

	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, 14, 16, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min);
	m_fileData.size = m_SimpleDataGeneration.m_numberOfPoints;
	m_fileData.data = m_SimpleDataGeneration.m_pointsDataANCAS;

	//create header for ancas
	m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
	m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
	//copy the header to the buffer


	//create the test parameters
	m_params.catchPolynomialDegree = 3;
	m_params.numberOfPopints = m_fileData.size;
	m_params.testedAlgorithm = TestParameters::Algorithm::SBO_ANCAS;
	m_params.catchRootsAlg = TestParameters::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "STARLINK_NANOFF_B");
#else
	// Standard C function, less safe but portable
	strncpy(m_params.testName, "STARLINK_NANOFF_B", MAX_TEST_NAME_SIZE);
	m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	m_params.testID = testID++;
	m_params.numberOfRuns = 1;

	m_params.TOLd = 0.00001;
	m_params.TOLt = 0.01;

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
	m_state = StateStart;

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
		std::left << std::setw(25) << results.testName
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
	std::cout << std::left << std::setw(25) << "Test Name"
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


	std::cout << std::left << std::setw(25) << std::setfill('-') << ""
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
