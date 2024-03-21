#include "TestManager.h"
#include "Factory.h"
#include <string.h>







TestManager::TestManager(ITimer* timer) : m_timer(timer)
{

}

TestManager::~TestManager()
{

}

TestResults::TestResult TestManager::runTest(TestParameters::TestRecipe params, TcaCalculation::sPointData * pointsData)
{
	TestResults::TestResult results = { 0 };
	ITcaAlgorithm* Algoritm;

	results.degree = params.catchPolynomialDegree;
	results.testedAlgorithm = params.testedAlgorithm;
	results.catchRootsAlg = params.catchRootsAlg;
	results.testID = params.testID;
	results.numberOfRuns = params.numberOfRuns;
	memcpy(results.testName, params.testName, MAX_TEST_NAME_SIZE);

	switch (params.testedAlgorithm)
	{
	default:
	case TestParameters::Algorithm::ANCAS:
		Algoritm = Factory::getReference()->getANCAS();
		break;
	case TestParameters::Algorithm::CATCH:
		Algoritm = Factory::getReference()->getCATCH(params.catchRootsAlg, params.catchPolynomialDegree);
		break;
	case TestParameters::Algorithm::SBO_ANCAS:
		Algoritm = Factory::getReference()->getSboAncas(params.elsetrec1, params.elsetrec2, params.startTime1Min, params.startTime2Min, params.TOLd, params.TOLt);
		break;
	}

	m_timer->startTimer();

	results.tca = Algoritm->runAlgorithm(pointsData, params.numberOfPopints - 1);

	m_timer->stopTimer();

	results.runTimeMicro = m_timer->getTimeInMicroSec();
	long double runTimeMicro;
	long double avgTimeMicro = results.runTimeMicro;
	long double minTimeMicro = results.runTimeMicro;
	//Do additional iterations
	TcaCalculation::TCA tca;

	for (int i = 1; i < params.numberOfRuns;i++)
	{
		switch (params.testedAlgorithm)
		{
		default:
		case TestParameters::Algorithm::ANCAS:
			Algoritm = Factory::getReference()->getANCAS();
			break;
		case TestParameters::Algorithm::CATCH:
			Algoritm = Factory::getReference()->getCATCH(params.catchRootsAlg, params.catchPolynomialDegree);
			break;
		case TestParameters::Algorithm::SBO_ANCAS:
			Algoritm = Factory::getReference()->getSboAncas(params.elsetrec1, params.elsetrec2, params.startTime1Min, params.startTime2Min, params.TOLd, params.TOLt);
			break;
		}
		m_timer->startTimer();

		tca = Algoritm->runAlgorithm(pointsData, params.numberOfPopints);

		m_timer->stopTimer();

		runTimeMicro = m_timer->getTimeInMicroSec();

		if (runTimeMicro < minTimeMicro)
		{
			minTimeMicro = runTimeMicro;
		}
		avgTimeMicro += runTimeMicro;
	}
	results.avgTimeMicro = avgTimeMicro / params.numberOfRuns;
	results.minTimeMicro = minTimeMicro;
	return results;
}

