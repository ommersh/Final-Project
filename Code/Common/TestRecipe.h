#ifndef TEST_RECIPE_H
#define TEST_RECIPE_H

#include <string>
#include "Enums/AlgorithmEnum.h"
#include "../../SGP4/CPP/SGP4.h"


static const int MAX_TEST_NAME_SIZE = 80;

/// <summary>
/// Parameters for running a test with a TCA finding algorithm
/// </summary>
struct TestRecipe {

	unsigned int testID;
	char testName[MAX_TEST_NAME_SIZE];
	//Test data/Input
	int catchPolynomialDegree;			// Degree of the polynomial, should be numberOfPointsPerSegment - 1
	int	numberOfPointsPerSegment;		// Use for data generations
	AlgorithmsEnums::CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
	AlgorithmsEnums::Algorithm testedAlgorithm;
	unsigned int numberOfIterations;
	int TminFactor; //2/4/8 
	double timeIntervalSizeSec;
	double TOLd;
	double TOLt;

	//Internal Data only(NOT FOR GUI)
	elsetrec elsetrec1;
	elsetrec elsetrec2;
	double startTime1Min;
	double startTime2Min;

	//Internal Data/Output/Display
	int numberOfPoints;
	double segmentSizeSec;

};

#endif