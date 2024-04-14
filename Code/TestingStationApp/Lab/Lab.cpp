#include "lab.h"
#include "SimpleDataGeneration.h"
    Lab::Lab() : m_databaseManager("Tests.db"),
        m_commManager(&m_testingStationLocalSimCommChannel),
        m_resultManager(),
        m_dataGenerator(),
        m_testManager()
    {
        m_testManager.init(m_resultManager, m_commManager);
        if (!m_databaseManager.createTables()) {
            std::cerr << "Failed to create tables." << std::endl;
        }
    }

    Lab::~Lab() {}

    Lab& Lab::GetInstance() {
        static Lab instance;
        return instance;
    }

    TestInfo Lab::GetTestInfo(int testId) {
        TestInfo testInfo = m_databaseManager.getTestInfo(testId);
        return testInfo;
    }

    void Lab::DeleteTest(int testId) {
        m_databaseManager.deleteTest(testId);
    }

    int Lab::CreateTest(std::string name, double timeInterval, int iterations, AlgorithmsEnums::Algorithm alg, AlgorithmsEnums::CatchRootsAlg catchRootAlg, int catchPolynomDeg, int numOfTimePoints, std::string elemDataOne, std::string elemDataTwo, SatelliteDataFormat format) {
        TestRecipe recipe = TestRecipe();
        recipe.timeIntervalSizeSec = timeInterval;
        recipe.numberOfIterations = iterations;
        recipe.testedAlgorithm = alg;
        recipe.catchPolynomialDegree = catchPolynomDeg;
        recipe.numberOfPoints = numOfTimePoints;
        recipe.TminFactor = 2;
        //recipe.TOLd; TODO add to info
        //recipe.TOLt; TODO add to info

        TestInfo testInfo = TestInfo();
        //testInfo.name = name;
        strncpy_s(testInfo.name, name.c_str(), sizeof(name) - 1);

        testInfo.recipe = recipe;
        testInfo.status = TestStatus::NotStarted;
        testInfo.format = format;

        strncpy_s(testInfo.firstElemData, elemDataOne.c_str(), sizeof(elemDataOne) - 1);
        strncpy_s(testInfo.secondElemData, elemDataTwo.c_str(), sizeof(elemDataTwo) - 1);

        //TODO - copy name to the test recipe
        
        //testInfo.firstElemData = elemDataOne;
        //testInfo.secondElemData = elemDataTwo;

        

        // Data generation
        // The data generator need to get the following references to return the values - For the recipe
        // elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1Min, double& startDataElem2Min, double& GammaSec
        
        //Create unique pointer and don't free the memory - the test manager will delete it if needed
        int arraySize = recipe.numberOfPoints;
        TcaCalculation::sPointData* pointsData = new TcaCalculation::sPointData[arraySize];

        m_databaseManager.createTest(testInfo);
        std::cout << testInfo.recipe.testID << std::endl;
        //todo: send test to card
        //TODO - Via Test Manager? place the recipe in the test queue
        //The test manager should have a different thread - running an
        m_testManager.PlaceTestInQueue(recipe, pointsData, arraySize);

        return testInfo.recipe.testID;

    }

    std::set<int> Lab::getAllTestIds()
    {
        return m_databaseManager.getAllTestIds();
    }


    Lab& Lab::operator=(const Lab&)
    {
        return *this;
    }


