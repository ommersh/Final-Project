#ifndef SHIELD_WinTcpCommChannel_H    // Check if the symbol SHIELD_WinTcpCommChannel_H is not defined
#define SHIELD_WinTcpCommChannel_H    // Define the symbol SHIELD_WinTcpCommChannel_H
#include "ICommChannel.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class WinTcpCommChannel : public ICommChannel
{
public:
	WinTcpCommChannel();
	virtual ~WinTcpCommChannel();

	bool init(std::string localIp, int localPort, std::string destIp, int destPort);

	/// <summary>
	/// Get the next message, return false if no message received
	/// </summary>
	/// <param name="buffer">Buffer for the outbut, for the received message</param>
	/// <param name="maxSize">The buffer size, the maximun number of bytes we can copy into the buffer</param>
	/// <param name="size">The actual message size copied into the buffer</param>
	/// <returns>Return true if a message received and false if not</returns>
	virtual bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
	/// <summary>
	/// Send a message
	/// </summary>
	/// <param name="buffer">Input buffer, message to send</param>
	/// <param name="size">The size of the message we send</param>
	virtual bool sendMessage(unsigned char* buffer, unsigned int size) override;
	/// <summary>
	/// Reset the channel between messages
	/// </summary>
	virtual void reset() override;


private:
	SOCKET listenSock;  // Socket for listening (server)
	SOCKET connectSock;  // Socket for connecting (client)
	WSADATA wsaData;
	sockaddr_in localAddr;  // Local address for server
	sockaddr_in destAddr;  // Remote address for client
	bool initServer();
	bool initClient();
	std::string m_localIp;
	int m_localPort;
	std::string m_destIp;
	int m_destPort;
};
#endif //SHIELD_WinTcpCommChannel_H
