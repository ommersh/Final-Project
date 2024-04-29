#ifndef SHIELD_WinUdpCommChannel_H    // Check if the symbol SHIELD_WinUdpCommChannel_H is not defined
#define SHIELD_WinUdpCommChannel_H    // Define the symbol SHIELD_WinUdpCommChannel_H
#include "ICommChannel.h"
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")


class WinTUdpCommChannel : public ICommChannel
{
public:
	WinTUdpCommChannel();
	virtual ~WinTUdpCommChannel();

	bool Init(std::string localIp, int localPort, std::string destIp, int destPort);

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
	/// <summary>
	/// Check if we are connected successfully
	/// </summary>
	/// <returns></returns>
	virtual bool checkConnection() override;

private:
	SOCKET localSocket;
	sockaddr_in destAddr;
};
#endif //SHIELD_WinUdpCommChannel_H
