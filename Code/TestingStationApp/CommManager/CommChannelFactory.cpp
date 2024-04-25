#include "CommChannelFactory.h"




CommChannelFactory& CommChannelFactory::GetInstance() {
	static CommChannelFactory instance;
	return instance;
}

ICommChannel& CommChannelFactory::getCommChannel(CommChannelConfig::TestingStationCommChannelType type)
{
	switch (type)
	{
	case CommChannelConfig::Udp:
		m_WinTUdpCommChannel.Init(CommChannelConfig::LOCAL_IP_ADDRESS, CommChannelConfig::LOCAL_PORT, CommChannelConfig::DEST_IP_ADDRESS, CommChannelConfig::DEST_PORT);
		return m_WinTUdpCommChannel;
		break;
	case CommChannelConfig::Tcp:
		m_tcpServer.Init(CommChannelConfig::LOCAL_IP_ADDRESS, CommChannelConfig::LOCAL_PORT);
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

};