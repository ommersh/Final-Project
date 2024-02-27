#include "Factory.h"


Factory::Factory()
{

}

Factory::~Factory()
{

}




Factory* Factory::getReferance()
{
	if (nullptr == m_referance)
	{
		m_referance = new Factory();
	}
	return m_referance;
}


ITimer* Factory::getTimer()
{
	if (nullptr == m_timer)
	{
		switch (AppConfigurationManager::timerType)
		{
		default:
		case AppConfiguration::TimerType::WinChrono:
			m_timer = new WinTimer();
			break;
		}
	}
	return m_timer;
}

IRootsFindAlg* Factory::getRootsFindAlg(TestParameters::CatchRootsAlg algType, int degree)
{
	IRootsFindAlg* rootsFindAlg = nullptr;
	switch (algType)
	{
	default:
	case TestParameters::CatchRootsAlg::EigenCompanionMatrix:
		rootsFindAlg = &m_companionMatrixRootsFinderEigen;
		break;
	}
	return rootsFindAlg;
}

CATCH* Factory::getCATCH(TestParameters::CatchRootsAlg algType, int degree)
{
	m_catch.init(getRootsFindAlg(algType, degree), degree);
	return &m_catch;
}

ANCAS* Factory::getANCAS()
{
	return &m_ancas;
}