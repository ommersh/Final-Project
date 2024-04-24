#ifndef CommChannelFactory_H
#define CommChannelFactory_H
#include "TestingStationLocalSimCommChannel.h"
#include "WinUdpCommChannel.h"
#include "TcpCommChannel.h"
#include <string>


namespace CommChannelConfig
{
	//const values for communication channel
	//TODO - get the values from app configuration
	static const int LOCAL_PORT = 8889;
	static const int DEST_PORT = 8888;
	static const std::string LOCAL_IP_ADDRESS = "172.22.192.1";//"127.0.0.1" "172.22.192.1";
	static const std::string DEST_IP_ADDRESS = "127.0.0.1";
	
	/// <summary>
	/// Type of comm channel
	/// </summary>
	enum TestingStationCommChannelType
	{
		LocalSim,
		Udp,
		Tcp,
	};


}

/// <summary>
/// Singleton factory class, create and initialize a commChannel based on given input and configurations
/// </summary>
class CommChannelFactory
{
public:
	/// <summary>
	/// Get the factory instance
	/// </summary>
	/// <returns></returns>
	static CommChannelFactory& GetInstance();
	/// <summary>
	/// Get the comm channel object
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	ICommChannel& getCommChannel(CommChannelConfig::TestingStationCommChannelType type);
private:
	/// <summary>
	/// Private constructor
	/// </summary>
	CommChannelFactory();


	//Possible comm channels
	TestingStationLocalSimCommChannel m_testingStationLocalSimCommChannel;
	WinTUdpCommChannel m_WinTUdpCommChannel;
	TCPServer m_tcpServer;
};

#endif // CommChannelFactory_H