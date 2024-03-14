#include "TestManager.h"
#include "Factory.h"







TestManager::TestManager(ITimer* timer, IMemoryMonitor* memoryMonitor) 
	: m_timer(timer),m_memoryMonitor(memoryMonitor)
{

}

TestManager::~TestManager()
{

}

TestResults::TestResult TestManager::runTest(TestParameters::TestRecipe params, TcaCalculation::sPointData * pointsData)
{
	TestResults::TestResult results = { 0 };
	ITcaAlgorithm* Algoritm;
	switch (params.testedAlgorithm)
	{
	default:
	case TestParameters::Algorithm::ANCAS:
		Algoritm = Factory::getReference()->getANCAS();
		break;
	case TestParameters::Algorithm::CATCH:
		Algoritm = Factory::getReference()->getCATCH(params.catchRootsAlg, params.degree);
		break;
	}

	m_memoryMonitor->startMonitoring();
	m_timer->startTimer();

	results.tca = Algoritm->runAlgorithm(pointsData, params.numberOfPopints);

	m_timer->stopTimer();

	results.runTimeMicro = m_timer->getTimeInMicroSec();
	results.maxMemoryUsed = m_memoryMonitor->stopMonitoringAndGetPeakUsage();

	return results;
}

