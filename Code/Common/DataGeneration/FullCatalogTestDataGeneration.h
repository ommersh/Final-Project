#ifndef SHIELD_FullCatalogTestDataGeneration_H    // Check if the symbol SHIELD_FullCatalogTestDataGeneration_H is not defined
#define SHIELD_FullCatalogTestDataGeneration_H    // Define the symbol SHIELD_FullCatalogTestDataGeneration_H

#include "TcaCalculation.h"
#include "DataGenerator.h"
#include "SimpleDataGeneration.h"
#include "FileReader.h"
#include "CommonStructures.h"
#include "Factory.h"


#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

typedef enum
{
    eAllWithAll,
    eOneWithAll
}FullCatalogTestVariation;

//Test Constants
static const int FIRST_SEGMENT_SIZE = 16;// 4;
static const int LAST_SEGMENT_SIZE = 16;// CATCH_MAX_DEGREE + 1;
static const int NUMBER_OF_ITERATIONS = 1;
static const int NUMBER_OF_DAYS = 7;

//Tolt 10-5 Told 10-9
static const double SBO_ANCAS_TOL_D_KM = 10e-9;
static const double SBO_ANCAS_TOL_T_SEC = 10e-5;

typedef enum
{
    eStartCase,
    eStartVariation,
    eAncas,
    eCatch,
    eSboAncas,
    eEndVariation,
    eEnded
}FullCatalogTestDataGenerationState;

class FullCatalogTestDataGeneration
{
private:
    DataGenerator m_dataGenerator;
    SimpleDataGeneration m_simpleDataGeneration;
public:
    bool init(const std::string& filePath);

    void getNextTestData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);
    void handleTestResults(TestResults::TestResult results);
protected:
    void getAncasData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);
    void getSboAncasData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);
    void getCatchData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);

    void generateDataSet();
    void clearMemory();

    void initElsetrecObjects();
    void moveToTheNextTest();

    void calculateWithSmallTimestep(double timePoint);

    //Test parameters
    FullCatalogTestVariation m_testVariation;
    int m_numberOfPointsInSegment;
    int m_firstObjectIndex;
    int m_secondObjectIndex;

    int m_numberOfiterations;
    int m_numberOfDays;

    double m_sboAncasTolDKm;
    double m_sboAncasTolTSec;
    bool m_calculateWithSmallTimestep;
    double m_timeStepSec;
    double m_timeIntervalSec;

    //Test data
    unsigned int m_testID;
    elsetrec m_elsetrec1;
    elsetrec m_elsetrec2;
    double m_startDataElem1;
    double m_startDataElem2;
    char m_testName[MAX_TEST_NAME_SIZE];
    int m_catalogSize;
    FullCatalogTestDataGenerationState m_state;
    unsigned int m_numberOfCases;
    unsigned int m_casesCounter;
    bool m_sboAncasRunning;
  

    //The input file
    std::ifstream m_inputFile;
    void getTLE(int position);
    int getCatalogSize();
    char m_tleLine1[130];
    char m_tleLine2[130];
    std::string m_name;

    void printPercentage();
};

#endif //SHIELD_SimpleDataGeneration_H