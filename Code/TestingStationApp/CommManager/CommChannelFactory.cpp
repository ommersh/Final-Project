#include "CommChannelFactory.h"
#include "ini.h"
#include "INIReader.h"
#include "EventLogger.h"


CommChannelFactory& CommChannelFactory::GetInstance() {
	static CommChannelFactory instance;
	return instance;
}

ICommChannel& CommChannelFactory::getCommChannel()
{
	switch (m_commChannelType)
	{
	case CommChannelConfig::Udp:
		m_WinTUdpCommChannel.Init(m_LocalIP, m_LocalPort, m_DestIP, m_DestPort);
		return m_WinTUdpCommChannel;
		break;
	case CommChannelConfig::Tcp:
		m_tcpServer.Init(m_LocalIP, m_LocalPort);
		return m_tcpServer;
		break;
	default:
	case CommChannelConfig::LocalSim:
		return m_testingStationLocalSimCommChannel;
		break;
	}
}

CommChannelFactory::CommChannelFactory()
{
	m_commChannelType = CommChannelConfig::COMM_CHANNEL_TYPE;
	m_LocalPort = CommChannelConfig::LOCAL_PORT;
	m_DestPort = CommChannelConfig::DEST_PORT;
	m_LocalIP = CommChannelConfig::LOCAL_IP_ADDRESS;
	m_DestIP = CommChannelConfig::DEST_IP_ADDRESS;
	LoadSettings();
}; 

void CommChannelFactory::LoadSettings()
{
	INIReader reader("TestingStationAppSettings.INI");
	std::string stringValue;
	//Log the app configuration
	std::string logString = "";

	LoadDefaults();
	if (reader.ParseError() < 0) {
		std::cerr << "Error parsing Config INI file!" << std::endl;
		logString = "Error parsing Config INI file!";
		EventLogger::getInstance().log(logString, "AppConfigurationManager");
		logString = "Working with default configurations!";
		EventLogger::getInstance().log(logString, "AppConfigurationManager");
	}
	else
	{

		stringValue = reader.Get("General", "CommChannelType", "LocalSimulation");
		if (stringValue == "LocalSimulation")
		{
			m_commChannelType = CommChannelConfig::TestingStationCommChannelType::LocalSim;
		}
		else if (stringValue == "Tcp")
		{
			m_commChannelType = CommChannelConfig::TestingStationCommChannelType::Tcp;
		}
		else if (stringValue == "Udp")
		{
			m_commChannelType = CommChannelConfig::TestingStationCommChannelType::Udp;
		}

		logString = "Comm Channel Type: " + stringValue;
		EventLogger::getInstance().log(logString, "CommChannelFactory");

		//IP and Ports
		m_LocalPort = reader.GetInteger("General", "SourcePort", CommChannelConfig::LOCAL_PORT);
		m_DestPort = reader.GetInteger("General", "DestinationPort", CommChannelConfig::DEST_PORT);
		m_LocalIP = reader.Get("General", "LocalIpAddress", CommChannelConfig::LOCAL_IP_ADDRESS);
		m_DestIP = reader.Get("General", "DestIpAddress", CommChannelConfig::DEST_IP_ADDRESS);

		logString = "Source Port: " + std::to_string(m_LocalPort);
		EventLogger::getInstance().log(logString, "AppConfigurationManager");
		logString = "Destination Port: " + std::to_string(m_DestPort);
		EventLogger::getInstance().log(logString, "AppConfigurationManager");
		logString = "Local Ip Address: " + m_LocalIP;
		EventLogger::getInstance().log(logString, "AppConfigurationManager");
		logString = "Destination Ip Address: " + m_DestIP;
		EventLogger::getInstance().log(logString, "AppConfigurationManager");

	}

}

void CommChannelFactory::LoadDefaults()
{
	m_commChannelType = CommChannelConfig::COMM_CHANNEL_TYPE;
	m_LocalPort = CommChannelConfig::LOCAL_PORT;
	m_DestPort = CommChannelConfig::DEST_PORT;
	m_LocalIP = CommChannelConfig::LOCAL_IP_ADDRESS;
	m_DestIP = CommChannelConfig::DEST_IP_ADDRESS;
}