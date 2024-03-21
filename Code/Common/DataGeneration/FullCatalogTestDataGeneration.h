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

struct TLE
{
    char line1[71];
    char line2[71];
};

//Test Constants
static const int FIRST_SEGMENT_SIZE = 4;
static const int LAST_SEGMENT_SIZE = CATCH_MAX_DEGREE + 1;
static const int NUMBER_OF_ITERATIONS = 1;
static const int NUMBER_OF_DAYS = 14;

static const double SBO_ANCAS_TOL_D_KM = 0.000001;
static const double SBO_ANCAS_TOL_T_SEC = 0.000001;

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

protected:
    void getAncasData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);
    void getSboAncasData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);
    void getCatchData(sFileData& fileData, TestParameters::TestRecipe& TestRecipe);

    void generateDataSet();
    void clearMemory();

    void initElsetrecObjects();

    FullCatalogTestDataGenerationState m_state;
    unsigned int m_testID;
    elsetrec m_elsetrec1;
    elsetrec m_elsetrec2;
    double m_startDataElem1;
    double m_startDataElem2;
    int m_numberOfPointsInSegment;
    char m_testName[MAX_TEST_NAME_SIZE];

    int m_catalogSize;
    unsigned int m_numberOfCases;
    unsigned int m_casesCounter;

    int m_firstObjectIndex;
    int m_secondObjectIndex;

    int m_numberOfiterations;
    int m_numberOfDays;

    double m_sboAncasTolDKm;
    double m_sboAncasTolTSec;

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