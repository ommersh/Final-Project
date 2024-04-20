#ifndef CommChannelFactory_H
#define CommChannelFactory_H
#include "TestingStationLocalSimCommChannel.h"
#include "WinUdpCommChannel.h"
#include "WinTcpCommChannel.h"
#include <string>


namespace CommChannelConfig
{
	//const values for communication channel
	//TODO - get the values from app configuration
	static const int LOCAL_PORT = 8889;
	static const int DEST_PORT = 8888;
	static const std::string LOCAL_IP_ADDRESS = "127.0.0.1";
	static const std::string DEST_IP_ADDRESS = "127.0.0.1";

	enum TestingStationCommChannelType
	{
		LocalSim,
		Udp,
		Tcp,
	};


}
class CommChannelFactory
{
public:
	static CommChannelFactory& GetInstance() {
		static CommChannelFactory instance;
		return instance;
	}
	ICommChannel& getCommChannel(CommChannelConfig::TestingStationCommChannelType type)
	{
		switch (type)
		{
		case CommChannelConfig::Udp:
			m_WinTUdpCommChannel.init(CommChannelConfig::LOCAL_IP_ADDRESS, CommChannelConfig::LOCAL_PORT, CommChannelConfig::DEST_IP_ADDRESS, CommChannelConfig::DEST_PORT);
			return m_WinTUdpCommChannel;
			break;
		case CommChannelConfig::Tcp:
			m_tcpServer.init(CommChannelConfig::LOCAL_IP_ADDRESS, CommChannelConfig::LOCAL_PORT);
			return m_tcpServer;
			break;
		default:
		case CommChannelConfig::LocalSim:
			return m_testingStationLocalSimCommChannel;
			break;
		}
	}
private:
	CommChannelFactory()
	{

	};


	//Possible comm channels
	TestingStationLocalSimCommChannel m_testingStationLocalSimCommChannel;
	WinTUdpCommChannel m_WinTUdpCommChannel;
	TCPServer m_tcpServer;
};

#endif // COMM_MANAGER_H