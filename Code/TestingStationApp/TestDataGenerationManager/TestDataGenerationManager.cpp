#include "TestDataGenerationManager.h"
#include <TcaCalculation.h>

/*
todo:
Methods: Create test(All test parameters: Format(tle/xml), algorithm1, algorithm2, tEnd, interval, polynomialDegree,  etc... )


*/

void TestDataGenerationManager::CreateTest(TestRecipe recipe)
{
    elsetrec orbElem1 = elsetrec();
    elsetrec orbElem2 = elsetrec();

    TestDataGenerationManager::ProcessOrbitingElement(recipe.firstElemData, orbElem1, recipe.format);
    TestDataGenerationManager::ProcessOrbitingElement(recipe.secondElemData, orbElem2, recipe.format);

    double gamma = DataGenerator::GetGamma(orbElem1, orbElem2);

    
    int timePointsArrLength = 1;/*todo: Generate timePointsArr fpr both elements according to choosen algorithm*/
    TcaCalculation::sPointData* elementsVectors = new  TcaCalculation::sPointData[timePointsArrLength];

    TestDataGenerationManager::GeneratePointsByAlgorithm(recipe.catchPolynomDeg, recipe.timeInterval, gamma, elementsVectors, recipe.alg);

    GenerateTestData(timePointsArrLength, orbElem1, orbElem2, elementsVectors);

    //todo save the test in some db
}

void TestDataGenerationManager::GenerateTestData(int timePointsArrLength, elsetrec& elsetrec1, elsetrec& elsetrec2, TcaCalculation::sPointData elementsVectors[]) {

    m_dataGenerator.CalculateRelativeVectorsForTwoElements(timePointsArrLength, elsetrec1, elsetrec2, elementsVectors);

}

void TestDataGenerationManager::findTCA() {

}



void TestDataGenerationManager::ProcessOrbitingElement(std::string& orbElemData, elsetrec& orbitingElement, SatelliteDataFormat format) {
    if (format == SatelliteDataFormat::XML)
    {
        m_dataGenerator.InitOrbitalElementsFromXml(orbElemData, orbitingElement);
    }
    else if (format == SatelliteDataFormat::Text)
    {
        char firstLine[130];
        char secondLine[130];
        SplitTLEString(orbElemData, firstLine, secondLine);
        m_dataGenerator.InitOrbitalElementsFromTLE(firstLine, secondLine, orbitingElement);
    }
    else
    {
        /* return error*/
    }
}

void TestDataGenerationManager::GenerateTimePointsForAncas(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[])
{
    m_dataGenerator.GenerateTimePointForAncas(n, tEnd, gamma, elementsVectors);
}

void TestDataGenerationManager::GenerateTimePointsForCatch(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[])
{
    m_dataGenerator.GenerateTimePointForCatch(n, tEnd, gamma, elementsVectors);
}



void TestDataGenerationManager::SplitTLEString(std::string tleString, char firstLine[130], char secondLine[130]) {
    // Find the position of the newline character that separates the two lines
    size_t pos = tleString.find('\n');
    if (pos == std::string::npos) {
        std::cerr << "Invalid TLE string format: Unable to find newline character." << std::endl;
        return;
    }

    // Copy the first line of the TLE string into the firstLine array
    size_t firstLineLength = std::min(pos, static_cast<size_t>(129)); // 129 characters + null terminator
    std::memcpy(firstLine, tleString.c_str(), firstLineLength);
    firstLine[firstLineLength] = '\0'; // Null-terminate the first line

    // Copy the second line of the TLE string into the secondLine array
    size_t secondLineLength = std::min(tleString.size() - pos - 1, static_cast<size_t>(129)); // 129 characters + null terminator
    std::memcpy(secondLine, tleString.c_str() + pos + 1, secondLineLength);
    secondLine[secondLineLength] = '\0'; // Null-terminate the second line
}


void TestDataGenerationManager::GeneratePointsByAlgorithm(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[], Algorithm alg) {
    // Call the corresponding method based on the algorithm
    (this->*methodMap[alg])(n, tEnd, gamma, elementsVectors);
}