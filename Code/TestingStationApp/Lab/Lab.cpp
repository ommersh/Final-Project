#include "lab.h"
#include "CommChannelFactory.h"
#include "EventLogger.h"
#include "AppVersion.h"

    Lab::Lab() : m_databaseManager("Tests.db"),
        m_commManager(),
        m_resultManager(),
        m_dataGenerator(),
        m_testManager()
    {
        EventLogger::getInstance().log("Lab Instance Created", "Lab");

        std::string versionInfo = "App Version: " +
            std::to_string(ProjectVersions::VERSION_MAJOR) + "." +
            std::to_string(ProjectVersions::VERSION_MINOR) + "." +
            std::to_string(ProjectVersions::VERSION_PATCH);
        EventLogger::getInstance().log(versionInfo, "Lab");


        m_commManager.Init(&(CommChannelFactory::GetInstance().getCommChannel()));
        EventLogger::getInstance().log("Comm Manager Initialized", "Lab");

        m_testManager.init(m_resultManager, m_commManager);
        if (!m_databaseManager.createTables()) {
            std::cerr << "Failed to create tables." << std::endl;
            EventLogger::getInstance().log("DatabaseManager:Failed to create tables", "Lab");
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

    int Lab::CreateTest(TestInfo testInfo){
        int returnValue = -1; //return testID on success and -1 on failure
        std::string logString = "";
        TestRecipe &recipe = testInfo.recipe;
        recipe.elsetrec1 = { 0 };
        recipe.elsetrec2 = { 0 };

        //Create unique pointer and don't free the memory - the test manager will delete it when needed
        TcaCalculation::sPointData* pointsData = nullptr;
        logString = "Creating Test";
        EventLogger::getInstance().log(logString, "Lab");

        if (true == m_dataGenerator.GenerateTestData(testInfo, &pointsData))
        {
            logString = "Test Data Generated";
            EventLogger::getInstance().log(logString, "Lab");
            if (true == m_databaseManager.createTest(testInfo))
            {
                //todo: send test to card
                //TODO - Via Test Manager? place the recipe in the test queue
                //The test manager should have a different thread - running an
                m_testManager.PlaceTestInQueue(recipe, pointsData, recipe.numberOfPoints);

                //delete[] pointsData;
                returnValue = testInfo.recipe.testID;
                logString = "Test Created And In Queue, Test ID " + std::to_string(testInfo.recipe.testID);
                EventLogger::getInstance().log(logString, "Lab");
            }
            else
            {
                logString = "Failed To Save Test To DB";
                EventLogger::getInstance().log(logString, "Lab");
                if (nullptr != pointsData)
                {
                    delete[] pointsData;
                }
            }
        }
        else
        {
            logString = "Failed To Generate Test Data";
            EventLogger::getInstance().log(logString, "Lab");
            if (nullptr != pointsData)
            {
                delete[] pointsData;
            }
        }
        return returnValue;
    }

    std::set<int> Lab::getAllTestIds()
    {
        return m_databaseManager.getAllTestIds();
    }


    Lab& Lab::operator=(const Lab&)
    {
        return *this;
    }


    bool Lab::updateTestResults(const TestInfo test)
    {
        return m_databaseManager.updateTestResults(test);
    }

    bool Lab::updateTestStatus(const TestStatus status, const unsigned int testId)
    {
        return m_databaseManager.updateTestStatus(status, testId);
    }