#include "TestDataGenerationManager.h"
//void TestDataGenerationManager::CreateTest(TestInfo& testInfo)
//{
//    elsetrec orbElem1 = elsetrec();
//    elsetrec orbElem2 = elsetrec();
//    TcaCalculation::sPointData* elementsVectors;
//
//    GenerateTestData(testInfo.recipe, orbElem1, orbElem2, &elementsVectors);
//}

void TestDataGenerationManager::GenerateTestData(TestInfo& testInfo, TcaCalculation::sPointData* elementsVectors[])
{
    CommonTestRecipe &recipe = testInfo.recipe;
    std::string firstElemData(testInfo.firstElemData);
    std::string secondElemData(testInfo.secondElemData);

    TestDataGenerationManager::ProcessOrbitingElement(firstElemData, recipe.firstElemObj, testInfo.format);
    TestDataGenerationManager::ProcessOrbitingElement(secondElemData, recipe.secondElemObj, testInfo.format);

    double gamma = DataGenerator::GetGamma(recipe.firstElemObj, recipe.secondElemObj);
    recipe.numOfTimePoints = ((int)(recipe.timeInterval / gamma)) * recipe.catchPolynomDeg;
    *elementsVectors = new  TcaCalculation::sPointData[recipe.numOfTimePoints];

    TestDataGenerationManager::GeneratePointsByAlgorithm(recipe.catchPolynomDeg, recipe.timeInterval, gamma, *elementsVectors, recipe.alg);
    m_dataGenerator.CalculateRelativeVectorsForTwoElements(recipe.numOfTimePoints, recipe.firstElemObj, recipe.secondElemObj, *elementsVectors);

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


void TestDataGenerationManager::GeneratePointsByAlgorithm(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[], AlgorithmsEnums::Algorithm alg) {
    // Call the corresponding method based on the algorithm
    return (this->*methodMap[alg])(n, tEnd, gamma, elementsVectors);
}