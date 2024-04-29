#include "TestManager.h"
#include "Factory.h"
#include <string.h>







TestManager::TestManager(ITimer* timer) : m_timer(timer)
{

}

TestManager::~TestManager()
{

}

TestResults::TestResult TestManager::runTest(TestRecipe params, TcaCalculation::sPointData * pointsData)
{
	TestResults::TestResult results = { 0 };
	ITcaAlgorithm* Algoritm;

	results.degree = params.catchPolynomialDegree;
	results.testedAlgorithm = params.testedAlgorithm;
	results.catchRootsAlg = params.catchRootsAlg;
	results.testID = params.testID;
	results.numberOfRuns = params.numberOfIterations;

	results.segmentSizeSec = params.segmentSizeSec;
	results.timeIntervalSizeSec = params.timeIntervalSizeSec;
	results.numberOfPointsPerSegment = params.numberOfPointsPerSegment;
	results.initialNumberOfPoints = params.numberOfPoints;
	results.TminFactor = params.TminFactor;

	memcpy(results.testName, params.testName, MAX_TEST_NAME_SIZE);

	switch (params.testedAlgorithm)
	{
	default:
	case AlgorithmsEnums::Algorithm::ANCAS:
		Algoritm = Factory::GetReference()->GetANCAS();
		break;
	case AlgorithmsEnums::Algorithm::CATCH:
		Algoritm = Factory::GetReference()->GetCATCH(params.catchRootsAlg, params.catchPolynomialDegree);
		break;
	case AlgorithmsEnums::Algorithm::SBO_ANCAS:
		Algoritm = Factory::GetReference()->GetSboAncas(params.elsetrec1, params.elsetrec2, params.startTime1Min, params.startTime2Min, params.TOLd, params.TOLt);
		break;
	case AlgorithmsEnums::Algorithm::SBO_ANCAS_ES:
		Algoritm = Factory::GetReference()->GetSboAncasEs(params.elsetrec1, params.elsetrec2, params.startTime1Min, params.startTime2Min, params.TOLd, params.TOLt);
		break;
	}

	m_timer->startTimer();

	results.tca = Algoritm->RunAlgorithm(pointsData, params.numberOfPoints - 1);

	m_timer->stopTimer();

	results.runTimeMicro = m_timer->getTimeInMicroSec();
	long double runTimeMicro;
	long double avgTimeMicro = results.runTimeMicro;
	long double minTimeMicro = results.runTimeMicro;
	//Do additional iterations
	TcaCalculation::TCA tca;

	for (int i = 1; i < params.numberOfIterations;i++)
	{
		switch (params.testedAlgorithm)
		{
		default:
		case AlgorithmsEnums::Algorithm::ANCAS:
			Algoritm = Factory::GetReference()->GetANCAS();
			break;
		case AlgorithmsEnums::Algorithm::CATCH:
			Algoritm = Factory::GetReference()->GetCATCH(params.catchRootsAlg, params.catchPolynomialDegree);
			break;
		case AlgorithmsEnums::Algorithm::SBO_ANCAS:
			Algoritm = Factory::GetReference()->GetSboAncas(params.elsetrec1, params.elsetrec2, params.startTime1Min, params.startTime2Min, params.TOLd, params.TOLt);
			break;
		case AlgorithmsEnums::Algorithm::SBO_ANCAS_ES:
			Algoritm = Factory::GetReference()->GetSboAncasEs(params.elsetrec1, params.elsetrec2, params.startTime1Min, params.startTime2Min, params.TOLd, params.TOLt);
			break;
		}
		m_timer->startTimer();

		tca = Algoritm->RunAlgorithm(pointsData, params.numberOfPoints);

		m_timer->stopTimer();

		runTimeMicro = m_timer->getTimeInMicroSec();

		if (runTimeMicro < minTimeMicro)
		{
			minTimeMicro = runTimeMicro;
		}
		avgTimeMicro += runTimeMicro;
	}
	results.avgTimeMicro = avgTimeMicro / params.numberOfIterations;
	results.minTimeMicro = minTimeMicro;
	return results;
}

