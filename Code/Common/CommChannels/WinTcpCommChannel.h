#ifndef SHIELD_WinTcpCommChannel_H    // Check if the symbol SHIELD_WinTcpCommChannel_H is not defined
#define SHIELD_WinTcpCommChannel_H    // Define the symbol SHIELD_WinTcpCommChannel_H

#include "ICommChannel.h"

#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include "SafeQueue.h"

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#endif //WIN32

class TCPServer : public ICommChannel
{
public:
    TCPServer();
    ~TCPServer();
    bool init(std::string serverIp, int serverPort);
    bool sendMessage(unsigned char* buffer, unsigned int size) override;
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
    void reset();
private:
    std::thread m_thisThread;


    void runServerService();
    unsigned int getMin(unsigned int a, unsigned int b);
    SOCKET listenSocket;
    SOCKET clientSocket;
    sockaddr_in serverAddress;
    SafeQueue< ReceivedMessage> m_receivedMessages;

};

class TCPClient : public ICommChannel
{
public:
    TCPClient();
    ~TCPClient();
    bool init(std::string serverIp, int serverPort);
    bool sendMessage(unsigned char* buffer, unsigned int size);
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
    void reset();
private:
    SOCKET m_socket;
    std::string m_serverIp;
    int m_serverPort;
    bool connectToServer(const std::string& serverIp, int serverPort);
};

#endif //SHIELD_WinTcpCommChannel_H
