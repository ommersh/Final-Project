#ifndef SHIELD_TestDefinitions_H    // Check if the symbol SHIELD_TestDefinitions_H is not defined
#define SHIELD_TestDefinitions_H    // Define the symbol SHIELD_TestDefinitions_H

#include "TcaCalculation.h"
#include "SGP4.h"

static const int MAX_TEST_NAME_SIZE = 25;

/// <summary>
/// Definitions related to the message between the TestedOBCApp and the TestingStationApp
/// </summary>
namespace MessagesDefinitions
{
	/// <summary>
	/// Unique Opcode for the TestRequestMessage.
	/// From the TestingStationApp to the TestedOBCApp
	/// </summary>
	static const unsigned short TestRequestMessageOpcode = 0x1234;
	/// <summary>
	/// Unique Opcode for the TestResultsMessage
	/// From the TestedOBCApp to the TestingStationApp
	/// </summary>
	static const unsigned short TestResultsMessageOpcode = 0x4321;

	struct MessageHeader
	{
		unsigned short opcode;
		unsigned int dataSize;
	};

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
}

namespace TestParameters
{
    enum CatchRootsAlg {
        EigenCompanionMatrix,
        ArmadilloCompanionMatrix
    };

    enum Algorithm {
        CATCH,
        ANCAS,
		SBO_ANCAS
    };

    /// <summary>
    /// Parameters for running a test with a TCA finding algorithm
    /// </summary>
    struct TestRecipe {
		//Test Data
        int catchPolynomialDegree;           // Degree of the polynomial
        CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
        Algorithm testedAlgorithm;
        int numberOfPoints;
		unsigned int numberOfIterations;
		double segmentSizeSec;
		int TminFactor;
		double timeIntervalSizeSec;
		int	numberOfPointsPerSegment;

		//For logging the data
		unsigned int testID;
		char testName[MAX_TEST_NAME_SIZE];

		//For SBO-ANCAS variations
		elsetrec elsetrec1;
		elsetrec elsetrec2;
		double startTime1Min;
		double startTime2Min;
		double TOLd;
		double TOLt;
    };
}


namespace TestResults
{
   
    struct TestResult {

		//The results and run time of the first run
        TcaCalculation::TCA tca;
		double runTimeMicro;

		//Repeated tests results
		long double avgTimeMicro;
		long double minTimeMicro;
		int numberOfRuns;

		//Test data
		char testName[MAX_TEST_NAME_SIZE];
		TestParameters::CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
		TestParameters::Algorithm testedAlgorithm;
		unsigned int testID;
		int degree;

		//Additional data
		double segmentSizeSec;
		double timeIntervalSizeSec;
		int	numberOfPointsPerSegment;
		int initialNumberOfPoints;
		int TminFactor;

    };
}











#endif //SHIELD_TestDefinitions_H