#ifndef TESTDATAGENERATIONMANAGER_H
#define TESTDATAGENERATIONMANAGER_H

#include <string>
#include <CPP/DataGenerator.h>
#include <AlgorithmEnum.h>
#include "../Enums/SatelliteDataFormatEnum.h"
#include <TestRecipe.h>
#include <TcaCalculation.h>
#include <map>

class TestDataGenerationManager {
public:
    TestDataGenerationManager() { m_dataGenerator = DataGenerator(); }
    void CreateTest(TestRecipe recipe);
    void GenerateTestData(int timePointsArrLength,  elsetrec& orbitingElement1, elsetrec& orbitingElement2, TcaCalculation::sPointData elementsVectors[]);
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