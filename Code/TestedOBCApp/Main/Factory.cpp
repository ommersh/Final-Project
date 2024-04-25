#include "Factory.h"
#include "EventLogger.h"

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

	m_configManager.Init("TestedObcAppSettings.INI");
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
	if (nullptr != m_commChannel)
	{
		delete m_commChannel;
	}
	if (nullptr != m_timer)
	{
		delete m_timer;
	}
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
Factory* Factory::GetReference()
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
ITimer* Factory::GetTimer()
{
	if (nullptr == m_timer)
	{
		switch (m_configManager.GetTimerType())
		{
		default:
		case AppConfiguration::TimerType::ChronoTimer:
			m_timer = new ChronoBasedTimer();
			break;
		}
	}
	return m_timer;
}

ICommChannel* Factory::GetCommChannel()
{
	if (nullptr == m_commChannel)
	{
		switch (m_configManager.GetCommChannelType())
		{
		default:
		case AppConfiguration::CommChannelType::LocalSimulation:
		{
			TestedOBCLocalSimulation* localSimulationCommChannel = new TestedOBCLocalSimulation();
			localSimulationCommChannel->Init("gpCatalog.txt");
			m_commChannel = localSimulationCommChannel;
		}
		break;
		case AppConfiguration::CommChannelType::Tcp:
		{
			TCPClient* TcpCommChannel = new TCPClient();
			if (false == TcpCommChannel->Init(m_configManager.GetDestIpAddress(), m_configManager.GetDestPort()))
			{
				std::cerr << "Failed to create/open the socket" << std::endl;
				EventLogger::getInstance().log("Failed to create/open the socket", "Factory");
			}
			m_commChannel = TcpCommChannel;
		}
		break;
#ifdef WIN32
		case AppConfiguration::CommChannelType::WinUdp:
		{
			WinTUdpCommChannel* winTcpCommChannel = new WinTUdpCommChannel();
			if (false == winTcpCommChannel->Init(m_configManager.GetLocalIpAddress(), m_configManager.GetSourcePort(), m_configManager.GetDestIpAddress(), m_configManager.GetDestPort()))
			{
				std::cerr << "Failed to create/open the socket" << std::endl;
				EventLogger::getInstance().log("Failed to create/open the socket", "Factory");
			}
			m_commChannel = winTcpCommChannel;
		}
		break;
#endif 
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
IRootsFindAlg* Factory::GetRootsFindAlg(AlgorithmsEnums::CatchRootsAlg algType, int degree)
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
CATCH* Factory::GetCATCH(AlgorithmsEnums::CatchRootsAlg algType, int degree)
{
	m_catch.Init(GetRootsFindAlg(algType, degree), degree);
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
ANCAS* Factory::GetANCAS()
{
	return &m_ancas;
}

SboAncas* Factory::GetSboAncas(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2, double TOLd, double TOLt)
{
	m_sboAncas.Init(GetSinglePointPropogator(elsetrec1, elsetrec2, startTime1, startTime2), TOLd, TOLt);
	return &m_sboAncas;
}

ISinglePointPropogator* Factory::GetSinglePointPropogator(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2)
{
	m_SGP4SinglePointGenerator.Init(elsetrec1, elsetrec2, startTime1, startTime2);
	return &m_SGP4SinglePointGenerator;
}

ResultsLogger* Factory::GetResultsLogger()
{
	return &m_resultsLogger;

}

AppConfigurationManager* Factory::GetConfigurationManager()
{
	return &m_configManager;
}
