#ifndef SHIELD_FullCatalogTestDataGeneration_H    // Check if the symbol SHIELD_FullCatalogTestDataGeneration_H is not defined
#define SHIELD_FullCatalogTestDataGeneration_H    // Define the symbol SHIELD_FullCatalogTestDataGeneration_H

#include "TcaCalculation.h"
#include "DataGenerator.h"
#include "SimpleDataGeneration.h"
#include "FileReader.h"
#include "CommonStructures.h"



#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "AppConfigurationManager.h"





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
    bool Init(const std::string& filePath);

    void getNextTestData(sFileData& fileData, TestRecipe& TestRecipe);
    void handleTestResults(TestResults::TestResult results);
protected:
    void getAncasData(sFileData& fileData, TestRecipe& TestRecipe);
    void getSboAncasData(sFileData& fileData, TestRecipe& TestRecipe);
    void getCatchData(sFileData& fileData, TestRecipe& TestRecipe);

    void generateDataSet();
    void clearMemory();

    void initElsetrecObjects();
    void moveToTheNextTest();

    void calculateWithSmallTimestep(double timePoint);

    void getNextPointsInIntervalTestData(sFileData& fileData, TestRecipe& TestRecipe);
    void getNextTimeIntervalTestData(sFileData& fileData, TestRecipe& TestRecipe);
    void getNextCatchDegreeTestData(sFileData& fileData, TestRecipe& TestRecipe);
    void stateUpdate();


    //Test parameters
    AppConfiguration::FullCatalogTestDataVariation m_testVariation;
    AppConfiguration::FullCatalogTestTypeVariation m_fullCatalogTestTypeVariation;

    int m_currentNumberOfPointsInSegment;
    int m_minNumberOfPointsInSegment;
    int m_maxNumberOfPointsInSegment;

    int m_firstObjectIndex;
    int m_secondObjectIndex;

    int m_numberOfiterations;
    int m_numberOfDays;
    int m_TMinFactor;

    double m_sboAncasTolDKm;
    double m_sboAncasTolTSec;
    bool m_calculateWithSmallTimestep;
    double m_timeStepSec;
    double m_timeIntervalSec;

    bool m_runAncas;
    bool m_runSboAncas;
    bool m_runCatch;

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