#include "Factory.h"

Factory* Factory::m_reference = nullptr; // Define the static member variable

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: Factory
//		
//	Description: Private Constructor
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
Factory::Factory() :
	m_timer(nullptr),
	m_commChannel(nullptr)
{

	m_configManager.init("TestedObcAppSettings.INI");
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: ~Factory
//		
//	Description: Destructor
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
Factory::~Factory()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getReference
//		
//	Description: Get a reference to the single instance of the factory
//					The first time the function is called - cretae the referance
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
Factory* Factory::getReference()
{
	if (nullptr == m_reference)
	{
		m_reference = new Factory();
	}
	return m_reference;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getTimer
//		
//	Description: Get the timer, for timing tests.
//					Based on the app configurations
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
ITimer* Factory::getTimer()
{
	if (nullptr == m_timer)
	{
		switch (m_configManager.getTimerType())
		{
		default:
		case AppConfiguration::TimerType::ChronoTimer:
			m_timer = new ChronoBasedTimer();
			break;
		}
	}
	return m_timer;
}

ICommChannel* Factory::getCommChannel()
{
	if (nullptr == m_commChannel)
	{
		switch (m_configManager.getCommChannelType())
		{
		default:
		case AppConfiguration::CommChannelType::LocalSimulation:
			TestedOBCLocalSimulation* localSimulationCommChannel = new TestedOBCLocalSimulation();
			localSimulationCommChannel->init("gpCatalog.txt");
			m_commChannel = localSimulationCommChannel;
			break;
		}
	}
	return m_commChannel;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getRootsFindAlg
//		
//	Description: Get appropriate IRootsFindAlg object based on the requested type;
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
IRootsFindAlg* Factory::getRootsFindAlg(AlgorithmsEnums::CatchRootsAlg algType, int degree)
{
	IRootsFindAlg* rootsFindAlg = nullptr;
	switch (algType)
	{
	default:
	case AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix:
		rootsFindAlg = &m_companionMatrixRootsFinderEigen;
		break;
	/*case AlgorithmsEnums::CatchRootsAlg::ArmadilloCompanionMatrix:
		rootsFindAlg = &m_companionMatrixRootsFinderAArmadillo;
		break;*/
	}
	return rootsFindAlg;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getCATCH
//		
//	Description: Get Catch object initialize to the requested degree and root finding algorithm.
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
CATCH* Factory::getCATCH(AlgorithmsEnums::CatchRootsAlg algType, int degree)
{
	m_catch.init(getRootsFindAlg(algType, degree), degree);
	return &m_catch;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function name: getANCAS
//		
//	Description: Get Ancas object, should be initialized.
//		
// 
//////////////////////////////////////////////////////////////////////////////////////////////
ANCAS* Factory::getANCAS()
{
	return &m_ancas;
}

SboAncas* Factory::getSboAncas(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2, double TOLd, double TOLt)
{
	m_sboAncas.init(getSinglePointPropogator(elsetrec1, elsetrec2, startTime1, startTime2), TOLd, TOLt);
	return &m_sboAncas;
}

ISinglePointPropogator* Factory::getSinglePointPropogator(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2)
{
	m_SGP4SinglePointGenerator.init(elsetrec1, elsetrec2, startTime1, startTime2);
	return &m_SGP4SinglePointGenerator;
}

ResultsLogger* Factory::getResultsLogger()
{
	return &m_resultsLogger;

}

AppConfigurationManager* Factory::getConfigurationManager()
{
	return &m_configManager;
}
