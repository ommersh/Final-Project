#include "lab.h"

    Lab::Lab() : m_databaseManager("Tests.db"),
        m_commManager(&m_testingStationLocalSimCommChannel),
        m_resultManager(),
        m_dataGenerator()
    {
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

    int Lab::CreateTest(std::string name, double timeInterval, int iterations, AlgorithmsEnums::Algorithm alg, int catchPolynomDeg, int numOfTimePoints, std::string elemDataOne, std::string elemDataTwo, SatelliteDataFormat format) {
        CommonTestRecipe recipe = CommonTestRecipe();
        recipe.timeInterval = timeInterval;
        recipe.iterations = iterations;
        recipe.alg = alg;
        recipe.catchPolynomDeg = catchPolynomDeg;
        recipe.numOfTimePoints = numOfTimePoints;

        TestInfo testInfo = TestInfo();
        //testInfo.name = name;
        strncpy_s(testInfo.name, name.c_str(), sizeof(name) - 1);

        testInfo.recipe = recipe;
        testInfo.status = TestStatus::NotStarted;
        testInfo.format = format;

        strncpy_s(testInfo.firstElemData, elemDataOne.c_str(), sizeof(elemDataOne) - 1);
        strncpy_s(testInfo.secondElemData, elemDataTwo.c_str(), sizeof(elemDataTwo) - 1);

        //testInfo.firstElemData = elemDataOne;
        //testInfo.secondElemData = elemDataTwo;

        TcaCalculation::sPointData* pointsData;

        //m_dataGenerator.GenerateTestData(testInfo, &pointsData);

        m_databaseManager.createTest(testInfo);
        std::cout << testInfo.recipe.testId << std::endl;
        return testInfo.recipe.testId;
        //todo: send test to card
    }

    std::set<int> Lab::getAllTestIds()
    {
        return m_databaseManager.getAllTestIds();
    }


    Lab& Lab::operator=(const Lab&)
    {
        return *this;
    }


