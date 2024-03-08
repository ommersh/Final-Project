#ifndef TESTDATAGENERATIONMANAGER_H
#define TESTDATAGENERATIONMANAGER_H

#include <string>
#include <CPP/DataGenerator.h>
#include <AlgorithmEnum.h>

class TestDataGenerationManager {
public:
    TestDataGenerationManager() { m_dataGenerator = DataGenerator(); }
    void createTestData(double* timeInMinutes, int timePointsArrLength,  elsetrec& orbitingElement1, elsetrec& orbitingElement2);
    void findTCA();
    void processOrbitingElement(char tle1[130], char tle2[130], elsetrec& orbitingElement);
    void processOrbitingElement(std::string& xmlFilePath, elsetrec& orbitingElement);
    void GenerateTimePointsForAncas(int n, double tEnd, double gamma);
    void GenerateTimePointsForCatch(int n, double tEnd, double gamma);

private:
    DataGenerator m_dataGenerator;
};

#endif