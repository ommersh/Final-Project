#include "TestManager.h"
#include "Factory.h"







TestManager::TestManager(ITimer* timer) : m_timer(timer)
{

}

TestManager::~TestManager()
{

}

TestResults::TestResult TestManager::runTest(TestParameters::TestRecipe params, TcaCalculation::sPointData * pointsData)
{
	TestResults::TestResult results = { 0 };

	switch (params.testedAlgorithm)
	{
	case TestParameters::Algorithm::ANCAS:
	{
		ANCAS* ancasAlg = Factory::getReference()->getANCAS();

		m_timer->startTimer();

		results.tca = ancasAlg->runAlgorithm(pointsData, params.numberOfPopints);
		
		m_timer->stopTimer();

		results.runTimeMicro = m_timer->getTimeInMicroSec();

	}
		break;

	case TestParameters::Algorithm::CATCH:
	{
		CATCH* catchAlg = Factory::getReference()->getCATCH(params.catchRootsAlg, params.degree);

		m_timer->startTimer();

		results.tca = catchAlg->runAlgorithm(pointsData, params.numberOfPopints);

		m_timer->stopTimer();

		results.runTimeMicro = m_timer->getTimeInMicroSec();

	}
		break;

	default:

		break;
	}

	return results;
}

