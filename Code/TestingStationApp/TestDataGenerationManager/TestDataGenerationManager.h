#ifndef TESTDATAGENERATIONMANAGER_H
#define TESTDATAGENERATIONMANAGER_H

#include <string>
#include "AlgorithmEnum.h"
#include "SatelliteDataFormatEnum.h"
#include "TestInfo.h"
#include <map>
#include "DataGenerator.h"

class TestDataGenerationManager {
public:
    TestDataGenerationManager() { m_dataGenerator = DataGenerator(); }
    //void CreateTest(TestInfo& recipe);
    bool GenerateTestData(TestInfo& testInfo, TcaCalculation::sPointData* elementsVectors[]);
    void findTCA();
    void ProcessOrbitingElement(std::string& tle, elsetrec& orbitingElement, SatelliteDataFormat format);
    void GeneratePointsByAlgorithm(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[], AlgorithmsEnums::Algorithm alg);

    TcaCalculation::TCA FindTcaWithSmallTimeStepArountPoint(elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1, double& startDataElem2, double stepSize, double timePoint, double segmentSizeSec);

private:
    DataGenerator m_dataGenerator;
    void GenerateTimePointsForAncas(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
    void GenerateTimePointsForCatch(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
    void SplitTLEString(std::string tleString, char firstLine[130], char secondLine[130]);

    std::map<AlgorithmsEnums::Algorithm, void (TestDataGenerationManager::*)(int, double, double, TcaCalculation::sPointData[])> methodMap{
    {AlgorithmsEnums::Algorithm::ANCAS, &TestDataGenerationManager::GenerateTimePointsForAncas},
    {AlgorithmsEnums::Algorithm::SBO_ANCAS, &TestDataGenerationManager::GenerateTimePointsForAncas},
    {AlgorithmsEnums::Algorithm::CATCH, &TestDataGenerationManager::GenerateTimePointsForCatch}
    };

};

#endif