#ifndef TEST_RECIPE_H
#define TEST_RECIPE_H

#include <string>
#include "Enums/AlgorithmEnum.h"
#include "../../SGP4/CPP/SGP4.h"


static const int MAX_TEST_NAME_SIZE = 25;

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

struct CommonTestRecipe {
    int testId;
    double timeInterval;
    int iterations;
    Algorithm alg;
    int catchPolynomDeg;
    elsetrec firstElemObj;
    elsetrec secondElemObj;
    int numOfTimePoints;
};

#endif