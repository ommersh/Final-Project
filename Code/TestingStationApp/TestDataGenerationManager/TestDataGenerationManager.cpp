#include "TestDataGenerationManager.h"
//void TestDataGenerationManager::CreateTest(TestInfo& testInfo)
//{
//    elsetrec orbElem1 = elsetrec();
//    elsetrec orbElem2 = elsetrec();
//    TcaCalculation::sPointData* elementsVectors;
//
//    GenerateTestData(testInfo.recipe, orbElem1, orbElem2, &elementsVectors);
//}

bool TestDataGenerationManager::GenerateTestData(TestInfo& testInfo, TcaCalculation::sPointData* elementsVectors[])
{
    bool dataGenerated = true;
    TestRecipe &recipe = testInfo.recipe;
    std::string firstElemData(testInfo.firstElemData);
    std::string secondElemData(testInfo.secondElemData);

    TestDataGenerationManager::ProcessOrbitingElement(firstElemData, recipe.elsetrec1, testInfo.format);
    TestDataGenerationManager::ProcessOrbitingElement(secondElemData, recipe.elsetrec2, testInfo.format);
    //check elsetrec pbjects for errors
    if (recipe.elsetrec1.error == 0 && recipe.elsetrec2.error == 0)
    {
        recipe.segmentSizeSec = DataGenerator::GetGamma(recipe.elsetrec1, recipe.elsetrec2, recipe.TminFactor);
        //For the first segment we use the full number of points, for each of the following we use one less
        recipe.numberOfPoints = 1 + (static_cast<int>(recipe.timeIntervalSizeSec / recipe.segmentSizeSec)) * (recipe.numberOfPointsPerSegment - 1);
        *elementsVectors = new  TcaCalculation::sPointData[recipe.numberOfPoints];
        if (nullptr != elementsVectors)
        {
            TestDataGenerationManager::GeneratePointsByAlgorithm(recipe.numberOfPointsPerSegment, recipe.timeIntervalSizeSec, recipe.segmentSizeSec, *elementsVectors, recipe.testedAlgorithm);
            dataGenerated &= m_dataGenerator.CalculateRelativeVectorsForTwoElements(recipe.numberOfPoints, recipe.elsetrec1, recipe.elsetrec2, *elementsVectors, testInfo.recipe.startTime1Min, testInfo.recipe.startTime2Min);
        }
        else
        {
            dataGenerated = false;
        }
    }
    else
    {
        dataGenerated = false;
    }
    return dataGenerated;
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

TcaCalculation::TCA TestDataGenerationManager::FindTcaWithSmallTimeStepArountPoint(elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1, double& startDataElem2, double stepSize, double timePoint, double segmentSizeSec)
{
    TcaCalculation::TCA tca;
    tca.time = 0;
    tca.distance = std::numeric_limits<double>::max();//initialize the distance to inf
    tca.numberOfPoints = 0;

    //test variables
    double r1[3], v1[3];
    double r2[3], v2[3];
    double tempDistance = 0;
    double timeInMinutes = 0;

    //calculate the time of the end of the test(in seconds)
    double testEndSec = timePoint + segmentSizeSec;
    double currentStepSec = timePoint - segmentSizeSec;

    while (currentStepSec < testEndSec)
    {
        //generate test data
        timeInMinutes = currentStepSec / 60;
        SGP4Funcs::sgp4(elsetrec1, startDataElem1 + timeInMinutes, r1, v1);
        SGP4Funcs::sgp4(elsetrec2, startDataElem2 + timeInMinutes, r2, v2);
        tca.numberOfPoints++;
        tempDistance = sqrt(pow((r1[0] - r2[0]), 2) + pow((r1[1] - r2[1]), 2) + pow((r1[2] - r2[2]), 2));
        if (tempDistance < tca.distance)
        {
            tca.distance = tempDistance;
            tca.time = currentStepSec;
        }

        currentStepSec += stepSize;
    }

    return tca;
}