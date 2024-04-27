#include "lab.h"
#include "CommChannelFactory.h"

    Lab::Lab() : m_databaseManager("Tests.db"),
        m_commManager(),
        m_resultManager(),
        m_dataGenerator(),
        m_testManager()
    {

        m_commManager.Init(&(CommChannelFactory::GetInstance().getCommChannel(CommChannelConfig::LocalSim)));
        
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

    int Lab::CreateTest(TestInfo testInfo){
        TestRecipe &recipe = testInfo.recipe;
        recipe.elsetrec1 = { 0 };
        recipe.elsetrec2 = { 0 };

        //Create unique pointer and don't free the memory - the test manager will delete it when needed
        TcaCalculation::sPointData* pointsData;

        m_dataGenerator.GenerateTestData(testInfo, &pointsData);

        m_databaseManager.createTest(testInfo);
        //todo: send test to card
        //TODO - Via Test Manager? place the recipe in the test queue
        //The test manager should have a different thread - running an
        m_testManager.PlaceTestInQueue(recipe, pointsData, recipe.numberOfPoints);

        //delete[] pointsData;
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


    bool Lab::updateTestResults(const TestInfo test)
    {
        return m_databaseManager.updateTestResults(test);
    }

    bool Lab::updateTestStatus(const TestStatus status, const unsigned int testId)
    {
        return m_databaseManager.updateTestStatus(status, testId);
    }