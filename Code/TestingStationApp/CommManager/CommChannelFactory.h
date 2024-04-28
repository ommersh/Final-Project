#ifndef CommChannelFactory_H
#define CommChannelFactory_H
#include "TestingStationLocalSimCommChannel.h"
#include "WinUdpCommChannel.h"
#include "TcpCommChannel.h"
#include <string>


namespace CommChannelConfig
{
	/// <summary>
	/// Type of comm channel
	/// </summary>
	enum TestingStationCommChannelType
	{
		LocalSim,
		Udp,
		Tcp,
	};

	//const default values for communication channel
	static const int LOCAL_PORT = 8889;
	static const int DEST_PORT = 8888;
	static const std::string LOCAL_IP_ADDRESS = "127.0.0.1";//"127.0.0.1" "172.22.192.1";
	static const std::string DEST_IP_ADDRESS = "127.0.0.1";
	static TestingStationCommChannelType COMM_CHANNEL_TYPE = LocalSim;



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
	ICommChannel& getCommChannel();
private:
	/// <summary>
	/// Private constructor
	/// </summary>
	CommChannelFactory();

	/// <summary>
	/// Load The Values From The INI File
	/// </summary>
	void LoadSettings();
	/// <summary>
	/// Initialize the values to the defaults
	/// </summary>
	void LoadDefaults();

	int m_LocalPort;
	int m_DestPort;
	std::string m_LocalIP;
	std::string m_DestIP;
	CommChannelConfig::TestingStationCommChannelType m_commChannelType;

	//Possible comm channels
	TestingStationLocalSimCommChannel m_testingStationLocalSimCommChannel;
	WinTUdpCommChannel m_WinTUdpCommChannel;
	TCPServer m_tcpServer;
};

#endif // CommChannelFactory_H