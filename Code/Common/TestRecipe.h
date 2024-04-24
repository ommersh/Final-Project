#ifndef TEST_RECIPE_H
#define TEST_RECIPE_H

#include <string>
#include "Enums/AlgorithmEnum.h"
#include "../../SGP4/CPP/SGP4.h"
#include <cstdint>

#pragma pack(push, 1)




static const int MAX_TEST_NAME_SIZE = 80;

/// <summary>
/// Parameters for running a test with a TCA finding algorithm
/// </summary>
struct TestRecipe {

	int32_t  testID;
	char testName[MAX_TEST_NAME_SIZE];
	//Test data/Input
	int32_t  catchPolynomialDegree;			// Degree of the polynomial, should be numberOfPointsPerSegment - 1
	int32_t  numberOfPointsPerSegment;		// Use for data generations
	AlgorithmsEnums::CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
	AlgorithmsEnums::Algorithm testedAlgorithm;
	uint32_t numberOfIterations;
	int32_t  TminFactor; //2/4/8 
	double timeIntervalSizeSec;
	double TOLd;
	double TOLt;

	//Internal Data only(NOT FOR GUI)
	elsetrec elsetrec1;
	elsetrec elsetrec2;
	double startTime1Min;
	double startTime2Min;

	//Internal Data/Output/Display
	int32_t numberOfPoints;
	double segmentSizeSec;

};
#pragma pack(pop)
#endif