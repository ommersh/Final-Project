#ifndef SHIELD_TestDefinitions_H    // Check if the symbol SHIELD_TestDefinitions_H is not defined
#define SHIELD_TestDefinitions_H    // Define the symbol SHIELD_TestDefinitions_H

#include "TcaCalculation.h"

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
        ANCAS
    };

    /// <summary>
    /// Parameters for running a test with a TCA finding algorithm
    /// </summary>
    struct TestRecipe {
        int degree;                         // Degree of the polynomial
        CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
        Algorithm testedAlgorithm;
        int numberOfPopints;
    };
}


namespace TestResults
{
   
    struct TestResult {
        TcaCalculation::TCA tca;
        double runTimeMicro;
		long long maxMemoryUsed;
    };
}











#endif //SHIELD_TestDefinitions_H