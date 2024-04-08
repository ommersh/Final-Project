#ifndef SHIELD_FullCatalogTestDataGeneration_H    // Check if the symbol SHIELD_FullCatalogTestDataGeneration_H is not defined
#define SHIELD_FullCatalogTestDataGeneration_H    // Define the symbol SHIELD_FullCatalogTestDataGeneration_H

#include "TcaCalculation.h"
#include "DataGenerator.h"
#include "SimpleDataGeneration.h"
#include "FileReader.h"
#include "CommonStructures.h"
#include "Factory.h"
#include "AppConfigurationManager.h"


#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>





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
    AppConfiguration::FullCatalogTestVariation m_testVariation;
    int m_currentNumberOfPointsInSegment;
    int m_minNumberOfPointsInSegment;
    int m_maxNumberOfPointsInSegment;

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
    double m_segmentSizeSec;
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