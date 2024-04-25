#include "UTSboAncas.h"
#include "TestUtils.h"


using ::testing::_;
using ::testing::DoAll; 
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::SetArgReferee;

//Test a real test case
TEST_F(SboAncasTestCase, TEST_test_case_STARLINK5447_UNICORN2N)
{
    double TOLd = 10e-9;
    double TOLt = 10e-5;

    double expectedResultsDistance = 0.13874966805571;
    double expectedResultsTime = 577578.397022249;

    //init and stuff
    getSboAncasData();
    int size = m_SimpleDataGeneration.m_numberOfPoints;
    data = m_SimpleDataGeneration.m_pointsDataANCAS;
    m_SGP4SinglePointGenerator.Init(elsetrec1, elsetrec2, startTime1Min, startTime2Min);
    sboAncas.Init(&m_SGP4SinglePointGenerator, TOLd, TOLt);

    //run Sbo Ancas
    if (size == -1)
    {
        //if we got here we couldnt create the data
        //and the test failed
        EXPECT_TRUE(false);
    }

    if (data != nullptr)
    {
        TCA tca = sboAncas.RunAlgorithm(data, size);
        //calculateWithSmallTimestep(tca.time);

        EXPECT_TRUE(TestUtils::CompareValues(tca.distance, expectedResultsDistance, TOLd));
        EXPECT_TRUE(TestUtils::CompareValues(tca.time, expectedResultsTime, TOLt));

    }
    else
    {
        //if we got here we couldnt create the data
        //and the test failed
        EXPECT_TRUE(false);
    }
}


void SboAncasTestCase::getSboAncasData()
{
	char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
	char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

	char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
	char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";
    elsetrec1 = { 0 };
    elsetrec2 = { 0 };
    startTime1Min = 0;
    startTime2Min = 0;
    double segmentSize;
	m_SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, 14, 16, elsetrec1, elsetrec2, startTime1Min, startTime2Min, segmentSize);
	

}


void SboAncasTestCase::calculateWithSmallTimestep(double timePoint)
{
    //Find the real TCA
    //resultsTca = m_SimpleDataGeneration.FindTcaWithSmallTimeStepArountPoint(elsetrec1, elsetrec2, startTime1Min, startTime2Min, 10e-5 / 10, timePoint, 5);
}

#include "ResultsLogger.h"
#include "ChronoBasedTimer.h";

//Test a real test case
TEST_F(SboAncasWorstCaseTest, TEST_alg_worst_case)
{
    static const bool run = false;
    if (run)
    {
        WorstCaseSboAncas sboAncas;
        double TOLd = 10e-9;
        double TOLt = 10e-5;
        double t_distance = 500;
        WorstCasePropogator worstCasePropogator;

        sboAncas.epsilon = TOLt;// 2.22507e-308;

        ResultsLogger logger("alg_worst_case.csv");
        ChronoBasedTimer timer;
        TestResults::TestResult results;
        results.avgTimeMicro = 0;
        results.catchRootsAlg = AlgorithmsEnums::EigenCompanionMatrix;
        results.degree = 0;
        results.minTimeMicro = 0;
        results.numberOfRuns = 0;
        results.runTimeMicro = 0;
        results.tca = { 0 };
        results.testedAlgorithm = AlgorithmsEnums::SBO_ANCAS;
        results.testID = 0;
        strcpy(results.testName, "worst_case");

        //init and stuff
        //getSboAncasData();
        int size = 4;
        TcaCalculation::sPointData testData[4] = { 0 };

        sboAncas.Init(&worstCasePropogator, TOLd, TOLt);

        for (t_distance = 0.01; t_distance < 1000; t_distance *= 10)
        {
            for (int i = 0; i < size; i++)
            {
                testData[i].time = i * t_distance;
            }

            sboAncas.middlePoint = testData[1].time;
            sboAncas.iterations = 0;

            timer.startTimer();
            results.tca = sboAncas.RunAlgorithm(testData, size);
            timer.stopTimer();


            results.runTimeMicro = timer.getTimeInMicroSec();
            results.testID++;
            results.avgTimeMicro = t_distance;
            results.minTimeMicro = sboAncas.epsilon;

            results.numberOfRuns = sboAncas.iterations;

            logger.log(results, TOLd, TOLt);
        }
    }
    //logger.~ResultsLogger();
    EXPECT_TRUE(true);

}