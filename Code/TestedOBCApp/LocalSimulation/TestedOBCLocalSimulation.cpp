#include "TestedOBCLocalSimulation.h"
#include <string.h>
#include "Utilities.h"

void TestedOBCLocalSimulation::init(const std::string& catalogFilePath)
{
	if (true == m_fullCatalogTestDataGeneration.init(catalogFilePath))
	{
		std::cout << std::endl << std::endl << "Running Local Simulation In Full Catalog Mode..." << std::endl << std::endl << std::endl;
		m_fullCatalog = true;
	}
	else
	{
		std::cout << std::endl << std::endl << "Running Local Simulation In Example Mode..." << std::endl << std::endl << std::endl;
		m_fullCatalog = false;
	}
}


void TestedOBCLocalSimulation::getTestData()
{
	if (m_fullCatalog == true)
	{
		m_fullCatalogTestDataGeneration.getNextTestData(m_fileData, m_params);

		m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
		m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
	}
	else
	{
		static unsigned char switchCounter = 0;

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
	}
}


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
bool TestedOBCLocalSimulation::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size)
{
	bool returnValue = false;
	//running the state machine
	switch (m_state)
	{
	case StateStart:
		//reset the offset and start immediately
		m_offset = 0;
		m_state = StateGetTestData;

	case StateGetTestData:
		//Get the data
		m_fileData.size = -1;
		getTestData();
		//if we failed to get the data reset the state machine
		if (m_fileData.size == -1)
		{
			//std::cout << "Failed to get data...\n";
			m_state = StateStart;
			break;
		}
		else
		{
			
			
			m_sizeToCopy = sizeof(MessageHeader) + sizeof(TestRecipe) + m_fileData.size * sizeof(TcaCalculation::sPointData);
			m_messageBuffer = new unsigned char[m_sizeToCopy];
			if (nullptr == m_messageBuffer)
			{
				returnValue = false;
				break;
			}
			int offset = sizeof(MessageHeader);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(&m_params), sizeof(TestRecipe));
			offset += sizeof(TestRecipe);
			memcpy(m_messageBuffer + offset, reinterpret_cast<unsigned char*>(m_fileData.data), m_fileData.size * sizeof(TcaCalculation::sPointData));
			//calculate crc
			m_header.crc = CRC32::calculate(m_messageBuffer + sizeof(MessageHeader), m_sizeToCopy - sizeof(MessageHeader));
			memcpy(m_messageBuffer, reinterpret_cast<unsigned char*>(&m_header), sizeof(MessageHeader));

			m_offset = 0;
		}
	case StateSendTestData:
		if (m_sizeToCopy < maxSize)
		{
			memcpy(buffer, m_messageBuffer + m_offset, m_sizeToCopy);
			*size = m_sizeToCopy;
			m_state = StateWaitForTestEnd;

		}
		else {
			memcpy(buffer, m_messageBuffer + m_offset, maxSize);
			m_sizeToCopy -= (maxSize);
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
#include "Factory.h"
//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: sendMessage
//		
//	Description:
//				Print the results based on the last parameters we "sent"
// 
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestedOBCLocalSimulation::sendMessage(unsigned char* buffer, unsigned int size)
{

	MessagesDefinitions::MessageHeader header;
	memcpy(reinterpret_cast<unsigned char*>(&header) , buffer , sizeof(MessagesDefinitions::MessageHeader));
	TestResults::TestResult testResults;
	memcpy(reinterpret_cast<unsigned char*>(&testResults), buffer + sizeof(MessagesDefinitions::MessageHeader),  sizeof(TestResults::TestResult));
	if (m_fullCatalog == false)
	{
		startPrint();
		switch (testResults.testedAlgorithm)
		{
		case AlgorithmsEnums::Algorithm::CATCH:
		{
			switch (testResults.catchRootsAlg)
			{
			case AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix:
				printResult("CATCH_Eigen", testResults);
				break;
			case AlgorithmsEnums::CatchRootsAlg::ArmadilloCompanionMatrix:
				printResult("CATCH_Armadillo", testResults);
				break;
			default:
				break;
			}
		}
		break;
		case AlgorithmsEnums::Algorithm::ANCAS:
			printResult("ANCAS", testResults);
			break;
		case AlgorithmsEnums::Algorithm::SBO_ANCAS:
			printResult("SBO_ANCAS", testResults);
			//calculateTheTcaWithSmallTimeStepAroundPoint(testResults.tca.time, 0.05);
			break;
		default:
			printResult("NA", testResults);
			break;
		};
	}
	else
	{
		m_fullCatalogTestDataGeneration.handleTestResults(testResults);
	}
	return true;
}

void TestedOBCLocalSimulation::calculateTheTcaWithSmallTimeStepAroundPoint(double timePoint, double segmentSize)
{
	//Find the real TCA
	Factory::getReference()->getTimer()->startTimer();
	TcaCalculation::TCA tca = m_SimpleDataGeneration.FindTcaWithSmallTimeStepArountPoint(m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min, m_params.TOLt/10, timePoint, segmentSize);
	Factory::getReference()->getTimer()->stopTimer();
	std::cout << std::setprecision(15)
		<< "Results with small time step:" << std::endl
		<< "Time step		: " << m_params.TOLt << std::endl
		<< "Distance		: " << tca.distance << std::endl
		<< "Time			: " << tca.time << std::endl
		<< "Number of points: " << tca.numberOfPoints << std::endl
		<< "Run time		: " << Factory::getReference()->getTimer()->getTimeInSec() << std::endl;
}

void TestedOBCLocalSimulation::calculateTheTcaWithSmallTimeStep()
{
	//Find the real TCA
	Factory::getReference()->getTimer()->startTimer();
	TcaCalculation::TCA tca = m_SimpleDataGeneration.FindTcaWithSmallTimeStep(14, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min, m_params.TOLt);
	Factory::getReference()->getTimer()->stopTimer();
	std::cout << std::setprecision(25)
		<< "Results with small time step:" << std::endl
		<< "Time step		: " << m_params.TOLt << std::endl
		<< "Distance		: " << tca.distance << std::endl
		<< "Time			: " << tca.time << std::endl
		<< "Number of points: " << tca.numberOfPoints << std::endl
		<< "Run time		: " << Factory::getReference()->getTimer()->getTimeInSec() << std::endl;
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
void TestedOBCLocalSimulation::getAncasData()
{
	char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
	char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

	char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
	char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";

	int numberOfDays = 14;
	int pointsPerSegment = 16;
	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, numberOfDays, pointsPerSegment, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min, m_params.segmentSizeSec);
	m_params.timeIntervalSizeSec = numberOfDays * 24 * 60 * 60;
	m_params.numberOfPointsPerSegment = pointsPerSegment;

	m_fileData.size = m_SimpleDataGeneration.m_numberOfPoints;
	m_fileData.data = m_SimpleDataGeneration.m_pointsDataANCAS;

	//create header for ancas
	m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
	m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
	//copy the header to the buffer


	//create the test parameters
	m_params.catchPolynomialDegree = 3;
	m_params.numberOfPoints = m_fileData.size;
	m_params.testedAlgorithm = AlgorithmsEnums::Algorithm::ANCAS;
	m_params.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "STARLINK5447_UNICORN2N");
#else
	// Standard C function, less safe but portable
	strncpy(m_params.testName, "STARLINK5447_UNICORN2N", MAX_TEST_NAME_SIZE);
	m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	m_params.testID = testID++;
	m_params.numberOfIterations = 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getCatchData
//		
//	Description: Read the data for CATCH from a file
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void TestedOBCLocalSimulation::getCatchData()
{
	char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
	char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

	char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
	char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";

	int numberOfDays = 14;
	int pointsPerSegment = 16;
	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, numberOfDays, pointsPerSegment, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min, m_params.segmentSizeSec);
	m_params.timeIntervalSizeSec = numberOfDays * 24 * 60 * 60;
	m_params.numberOfPointsPerSegment = pointsPerSegment;

	m_fileData.size = m_SimpleDataGeneration.m_numberOfPoints;
	m_fileData.data = m_SimpleDataGeneration.m_pointsDataCATCH;
	
	
	m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
	m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);

	m_params.catchPolynomialDegree = 15;
	m_params.numberOfPoints = m_fileData.size;
	m_params.testedAlgorithm = AlgorithmsEnums::Algorithm::CATCH;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "STARLINK5447_UNICORN2N");
#else
	// Standard C function, less safe but portable
	strncpy(m_params.testName, "STARLINK5447_UNICORN2N", MAX_TEST_NAME_SIZE);
	m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	m_params.testID = testID++;
	m_params.numberOfIterations = 1;
	//if (switchCounter++ % 2 == 0)
	//{
	m_params.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
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
void TestedOBCLocalSimulation::getSboAncasData()
{
	char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
	char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

	char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
	char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";

	int numberOfDays = 14;
	int pointsPerSegment = 16;
	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, numberOfDays, pointsPerSegment, m_params.elsetrec1, m_params.elsetrec2, m_params.startTime1Min, m_params.startTime2Min, m_params.segmentSizeSec);
	m_params.timeIntervalSizeSec = numberOfDays * 24 * 60 * 60;
	m_params.numberOfPointsPerSegment = pointsPerSegment;

	m_fileData.size = m_SimpleDataGeneration.m_numberOfPoints;
	m_fileData.data = m_SimpleDataGeneration.m_pointsDataANCAS;

	//create header for ancas
	m_header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
	m_header.dataSize = m_fileData.size * sizeof(TcaCalculation::sPointData);
	//copy the header to the buffer


	//create the test parameters
	m_params.catchPolynomialDegree = 3;
	m_params.numberOfPoints = m_fileData.size;
	m_params.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
	m_params.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(m_params.testName, MAX_TEST_NAME_SIZE, "STARLINK5447_UNICORN2N");
#else
	// Standard C function, less safe but portable
	strncpy(m_params.testName, "STARLINK5447_UNICORN2N", MAX_TEST_NAME_SIZE);
	m_params.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	m_params.testID = testID++;
	m_params.numberOfIterations = 1;

	m_params.TOLd = Factory::getReference()->getConfigurationManager()->getTOLd();
	m_params.TOLt = Factory::getReference()->getConfigurationManager()->getTOLt();

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: reset
//		
//	Description: Reset between messages
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void TestedOBCLocalSimulation::reset()
{
	m_state = StateStart;
	if (m_fullCatalog == false)
	{
		if (nullptr != m_fileData.data)
		{
			delete[] m_fileData.data;
		}
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
void TestedOBCLocalSimulation::printResult(string algName, TestResults::TestResult results)
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
void TestedOBCLocalSimulation::startPrint()
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
