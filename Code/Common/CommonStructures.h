#ifndef SHIELD_TestDefinitions_H    // Check if the symbol SHIELD_TestDefinitions_H is not defined
#define SHIELD_TestDefinitions_H    // Define the symbol SHIELD_TestDefinitions_H

#include "TcaCalculation.h"
#include "SGP4.h"
#include "AlgorithmEnum.h"
#include "TestRecipe.h"
#include <cstdint>

#pragma pack(push, 1)

namespace TestResults
{

	struct TestResult {

		//The results and run time of the first run
		TcaCalculation::TCA tca;
		double runTimeMicro;

		//Repeated tests results
		double avgTimeMicro;
		double minTimeMicro;
		int32_t numberOfRuns;

		//Test data
		char testName[MAX_TEST_NAME_SIZE];
		AlgorithmsEnums::CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
		AlgorithmsEnums::Algorithm testedAlgorithm;
		uint32_t testID;
		int32_t degree;

		//Additional data
		double segmentSizeSec;
		double timeIntervalSizeSec;
		int32_t	numberOfPointsPerSegment;
		int32_t initialNumberOfPoints;
		int32_t TminFactor;

	};
}


/// <summary>
/// Definitions related to the message between the TestedOBCApp and the TestingStationApp
/// </summary>
namespace MessagesDefinitions
{
	/// <summary>
	/// Unique Opcode for the TestRequestMessage.
	/// From the TestingStationApp to the TestedOBCApp
	/// </summary>
	static const uint16_t TestRequestMessageOpcode = 0x1234;
	/// <summary>
	/// Unique Opcode for the TestResultsMessage
	/// From the TestedOBCApp to the TestingStationApp
	/// </summary>
	static const uint16_t TestResultsMessageOpcode = 0x4321;

	struct MessageHeader
	{
		uint16_t opcode;
		uint32_t dataSize;
		uint32_t crc;//crc of the full message without the header
	};

	//size definitions
	static const uint16_t OPCODE_SIZE = sizeof(TestResultsMessageOpcode);
	static const uint16_t MESSAGE_HEADER_SIZE = sizeof(MessageHeader);

	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	//		TestRequestMessage description
	//		
	//		From the TestingStationApp to the TestedOBCApp
	//		The message structure is the following:
	// 
	// 			Bytes	Field
	// 			0-5		MessageHeader
	//			6-n		TestParams
	//			n-end	TestData(Array of sPointData)
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	//		TestResultsMessage description
	//		
	//		From the TestedOBCApp to the TestingStationApp
	//		The message structure is the following:
	// 
	// 			Bytes	Field
	// 			0-5		MessageHeader
	//			6-end	TestResult
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////
	struct TestResultsMessage {
		MessageHeader header;
		TestResults::TestResult results;
	};
}













#pragma pack(pop)

#endif //SHIELD_TestDefinitions_H