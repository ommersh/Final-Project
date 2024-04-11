#ifndef TESTDATAGENERATIONMANAGER_H
#define TESTDATAGENERATIONMANAGER_H

#include <string>
#include "../../Common/Enums/AlgorithmEnum.h"
#include "../Enums/SatelliteDataFormatEnum.h"
#include "../TestManager/ResultManager/TestInfo.h"
#include <map>
#include "../../../SGP4/CPP/DataGenerator.h"

class TestDataGenerationManager {
public:
    TestDataGenerationManager() { m_dataGenerator = DataGenerator(); }
    //void CreateTest(TestInfo& recipe);
    void GenerateTestData(TestInfo& testInfo, TcaCalculation::sPointData* elementsVectors[]);
    void findTCA();
    void ProcessOrbitingElement(std::string& tle, elsetrec& orbitingElement, SatelliteDataFormat format);
    void GeneratePointsByAlgorithm(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[], Algorithm alg);


private:
    DataGenerator m_dataGenerator;
    void GenerateTimePointsForAncas(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
    void GenerateTimePointsForCatch(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
    void SplitTLEString(std::string tleString, char firstLine[130], char secondLine[130]);

    std::map<Algorithm, void (TestDataGenerationManager::*)(int, double, double, TcaCalculation::sPointData[])> methodMap{
    {Algorithm::Ancas, &TestDataGenerationManager::GenerateTimePointsForAncas},
    {Algorithm::Catch, &TestDataGenerationManager::GenerateTimePointsForCatch}
    };

};

#endif