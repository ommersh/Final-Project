#include "TestManager.h"








TestManager::TestManager(ITimer& timer) : m_timer(timer)
{

}

TestManager::~TestManager()
{

}

TestResults::TestResult TestManager::runTest(TestParameters::TestParams params, TcaCalculation::sPointData * pointsData, double * timePoints, int numberOfPopints)
{
	TestResults::TestResult results = { 0 };

	switch (params.testedAlgorithm)
	{
	case TestParameters::Algorithm::ANCAS:
	{
		ANCAS ancasAlg;

		m_timer.startTimer();

		results.tca = ancasAlg.runAlgorithm(pointsData, timePoints, numberOfPopints);
		
		m_timer.stopTimer();

		results.runTimeMicro = m_timer.getTimeInMicroSec();

	}
		break;

	case TestParameters::Algorithm::CATCH:
	{
		IRootsFindAlg* rootsFinder = getRootsFindAlg(params.catchRootsAlg, params.degree);
		CATCH catchAlg(rootsFinder, params.degree);

		m_timer.startTimer();

		results.tca = catchAlg.runAlgorithm(pointsData, timePoints, numberOfPopints);

		m_timer.stopTimer();

		results.runTimeMicro = m_timer.getTimeInMicroSec();

		delete rootsFinder;
	}
		break;

	default:

		break;
	}

	return results;
}


IRootsFindAlg* TestManager::getRootsFindAlg(TestParameters::CatchRootsAlg algType, int degree)
{
	IRootsFindAlg* alg = nullptr;
	switch (algType)
	{
	case TestParameters::CatchRootsAlg::EigenCompanionMatrix:
		alg = new CompanionMatrixRootsFinder(degree);
		break;
		
	default:

		break;
	}
	return alg;
}
