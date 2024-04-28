#include "ResultManager.h"
#include <iomanip>
#include "Lab.h"


void ResultManager::UpdateTestResult(TestResults::TestResult results) {

	printTestResults(results);
	TestInfo testInfo = { 0 };
	//The updateTestResults only use the test results values, the test ID and the test status
	testInfo.recipe.testID = results.testID;
	//the results values
	//The results and run time of the first run
	testInfo.timeOfTcaFromStartingPointSec = results.tca.time;
	testInfo.distanceOfTcaKM = results.tca.distance;
	testInfo.numberOfPointsTheAlgUsed = results.tca.numberOfPoints;
	testInfo.runTimeMicro = results.runTimeMicro;

	//Repeated tests results - the iterations
	testInfo.avgRunTimeMicro = results.avgTimeMicro;
	testInfo.minRunTimeMicro = results.minTimeMicro;

	//testInfo.status = TestStatus::Completed;

	Lab::GetInstance().updateTestResults(testInfo);
}

void ResultManager::UpdateTestRealTcaResult(TcaCalculation::TCA tca, int testID)
{
	TestInfo testInfo = { 0 };
	testInfo.recipe.testID = testID;
	testInfo.realTCA = tca.time;
	testInfo.realDistance = tca.distance;
	Lab::GetInstance().updateRealTca(testInfo);
}

void ResultManager::TestCompleted(int testID)
{
	Lab::GetInstance().updateTestStatus(TestStatus::Completed, testID);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: printResult
//		
//	Description: Print the results in a table:
//		Test Name      |Test ID        |Alg Name       |Degree         |NumberOfPoints      |RunTime(sec)        |RunTime(microSec)   |NumberOfRuns   |avgTimeMicro   |minTimeMicro   |TCA distance		 |TCA time
//		---------------|---------------|---------------|---------------|--------------------|--------------------|--------------------|---------------|---------------|---------------|------------------|---------------
//		LEMUR2_COSMOS  | 0			   | ANCAS		   | 15			   | 6451				| 0.000676			 | 676				  | 100			  | 645.49		  | 627			  | 1.17159132160904 | 177095.670099459
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void ResultManager::printResult(string algName, TestResults::TestResult results)
{
	std::cout << std::setprecision(15) <<
		std::left << std::setw(25) << results.testName
		<< "|" << std::left << std::setw(15) << results.testID
		<< "|" << std::left << std::setw(15) << algName
		<< "|" << std::left << std::setw(15) << results.degree
		<< "|" << std::left << std::setw(20) << results.tca.numberOfPoints
		<< "|" << std::left << std::setw(20) << results.runTimeMicro / 1000000
		<< "|" << std::left << std::setw(20) << results.runTimeMicro

		<< "|" << std::left << std::setw(15) << results.numberOfRuns
		<< "|" << std::left << std::setw(15) << results.avgTimeMicro
		<< "|" << std::left << std::setw(15) << results.minTimeMicro


		<< "|" << std::left << std::setw(20) << results.tca.distance
		<< "|" << std::left << std::setw(20) << results.tca.time << std::endl;

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: startPrint
//		
//	Description: Print the start of the table:
//		Test Name      |Test ID        |Alg Name       |Degree         |NumberOfPoints      |RunTime(sec)        |RunTime(microSec)   |NumberOfRuns   |avgTimeMicro   |minTimeMicro   |TCA distance		 |TCA time
//		---------------|---------------|---------------|---------------|--------------------|--------------------|--------------------|---------------|---------------|---------------|------------------|---------------
//		LEMUR2_COSMOS  | 0			   | ANCAS		   | 15			   | 6451				| 0.000676			 | 676				  | 100			  | 645.49		  | 627			  | 1.17159132160904 | 177095.670099459
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void ResultManager::startPrint()
{
	std::cout << std::left << std::setw(25) << "Test Name"
		<< "|" << std::left << std::setw(15) << "Test ID"
		<< "|" << std::left << std::setw(15) << "Alg Name"
		<< "|" << std::left << std::setw(15) << "Degree"
		<< "|" << std::left << std::setw(20) << "NumberOfPoints"
		<< "|" << std::left << std::setw(20) << "RunTime(sec)"
		<< "|" << std::left << std::setw(20) << "RunTime(microSec)"

		<< "|" << std::left << std::setw(15) << "NumberOfRuns"
		<< "|" << std::left << std::setw(15) << "avgTimeMicro"
		<< "|" << std::left << std::setw(15) << "minTimeMicro"

		<< "|" << std::left << std::setw(20) << "TCA distance"
		<< "|" << std::left << std::setw(20) << "TCA time" << std::endl;


	std::cout << std::left << std::setw(25) << std::setfill('-') << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""

		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << ""

		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""
		<< "|" << std::left << std::setw(15) << ""

		<< "|" << std::left << std::setw(20) << ""
		<< "|" << std::left << std::setw(20) << "" << std::setfill(' ') << std::endl;

}

void ResultManager::printTestResults(TestResults::TestResult testResults)
{

	startPrint();
	switch (testResults.testedAlgorithm)
	{
	case AlgorithmsEnums::Algorithm::CATCH:
	{
		switch (testResults.catchRootsAlg)
		{
		case AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix:
			printResult("CATCH_Eigen", testResults);
			break;
		case AlgorithmsEnums::CatchRootsAlg::ArmadilloCompanionMatrix:
			printResult("CATCH_Armadillo", testResults);
			break;
		default:
			break;
		}
	}
	break;
	case AlgorithmsEnums::Algorithm::ANCAS:
		printResult("ANCAS", testResults);
		break;
	case AlgorithmsEnums::Algorithm::SBO_ANCAS:
		printResult("SBO_ANCAS", testResults);
		//calculateTheTcaWithSmallTimeStepAroundPoint(testResults.tca.time, 0.05);
		break;
	default:
		printResult("NA", testResults);
		break;
	};
}