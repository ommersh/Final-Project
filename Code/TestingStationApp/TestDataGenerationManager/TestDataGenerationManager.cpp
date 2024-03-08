#include "TestDataGenerationManager.h"

/*
todo:
Methods: Create test(All test parameters: Format(tle/xml), algorithm1, algorithm2, tEnd, interval, polynomialDegree,  etc... )


*/


void TestDataGenerationManager::GenerateTestData(double* timeInMinutes, int timePointsArrLength, elsetrec& elsetrec1, elsetrec& elsetrec2) {
    double* f = new double[timePointsArrLength];
    double* df = new double[timePointsArrLength];
    double** r1Arr = new double* [timePointsArrLength];
    double** r2Arr = new double* [timePointsArrLength];
    double** v1Arr = new double* [timePointsArrLength];
    double** v2Arr = new double* [timePointsArrLength];

    for (int i = 0; i < timePointsArrLength; ++i) {
        r1Arr[i] = new double[3];
        r2Arr[i] = new double[3];
        v1Arr[i] = new double[3];
        v2Arr[i] = new double[3];
    }

    m_dataGenerator.CalculateRelativeVectorsForTwoElements(timeInMinutes, timePointsArrLength, elsetrec1, elsetrec2, r1Arr, r2Arr, v1Arr, v1Arr, f, df);

}

void TestDataGenerationManager::findTCA() {

}



void TestDataGenerationManager::processOrbitingElement(char tle1[130], char tle2[130], elsetrec& orbitingElement) {
	m_dataGenerator.InitOrbitalElementsFromTLE(tle1, tle2, orbitingElement);
}

void TestDataGenerationManager::processOrbitingElement(std::string& xmlFilePath, elsetrec& orbitingElement) {
	m_dataGenerator.InitOrbitalElementsFromXml(xmlFilePath, orbitingElement);
}

void TestDataGenerationManager::GenerateTimePointsForAncas(int n, double tEnd, double gamma)
{
    m_dataGenerator.GenerateTimePointForAncas(n, tEnd, gamma);
}

void TestDataGenerationManager::GenerateTimePointsForCatch(int n, double tEnd, double gamma)
{
    m_dataGenerator.GenerateTimePointForCatch(n, tEnd, gamma);
}



