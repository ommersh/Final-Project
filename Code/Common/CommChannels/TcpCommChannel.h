#ifndef SHIELD_TcpCommChannel_H    // Check if the symbol SHIELD_TcpCommChannel_H is not defined
#define SHIELD_TcpCommChannel_H    // Define the symbol SHIELD_TcpCommChannel_H

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
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for close function
#endif //WIN32

#ifdef WIN32
/// <summary>
/// TCP Server. Manage the communication with a single client(Tested OBC App). 
/// For windows only
/// </summary>
class TCPServer : public ICommChannel
{
public:
    TCPServer();
    ~TCPServer();
    /// <summary>
    /// Initialize the server. 
    /// Return true on success
    /// </summary>
    /// <param name="serverIp"></param>
    /// <param name="serverPort"></param>
    /// <returns></returns>
    bool Init(std::string serverIp, int serverPort);
    /// <summary>
    /// Send a message to the client(if exist). 
    /// Return true on success
    /// </summary>
    /// <param name="buffer"></param>
    /// <param name="size"></param>
    /// <returns></returns>
    bool sendMessage(unsigned char* buffer, unsigned int size) override;
    /// <summary>
    /// Receive a message from the client(if exist). 
    /// Return true on success
    /// </summary>
    /// <param name="buffer">the received message output</param>
    /// <param name="maxSize">The buffer size</param>
    /// <param name="size">Return the received message size</param>
    /// <returns></returns>
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
    /// <summary>
    /// Reset the communication(Not the channel, for cleaning buffers and similar stuff)
    /// </summary>
    void reset();
private:
    /// <summary>
    /// thread running the server tasks
    /// </summary>
    std::thread m_thisThread;
    /// <summary>
    /// handle server tasks(connections, incoming messaages, disconnections)
    /// </summary>
    void runServerService();
    /// <summary>
    /// Server socket
    /// </summary>
    SOCKET listenSocket;
    /// <summary>
    /// Client socket
    /// </summary>
    SOCKET clientSocket;
    /// <summary>
    /// Server IP Address
    /// </summary>
    sockaddr_in serverAddress;
    /// <summary>
    /// Queue for the incoming messages
    /// </summary>
    SafeQueue< ReceivedMessage> m_receivedMessages;

};
#endif //WIN32
/// <summary>
/// TCP Client class, manage communication for a client. 
/// For both windows and non windows systems
/// </summary>
class TCPClient : public ICommChannel
{
public:
    TCPClient();
    ~TCPClient();
    /// <summary>
    /// Initialize the client connection. 
    /// Return true on success
    /// </summary>
    /// <param name="serverIp"></param>
    /// <param name="serverPort"></param>
    /// <returns></returns>
    bool Init(std::string serverIp, int serverPort);
    /// <summary>
    /// Send a message to the server. 
    /// Return true on success
    /// </summary>
    /// <param name="buffer"></param>
    /// <param name="size"></param>
    /// <returns></returns>
    bool sendMessage(unsigned char* buffer, unsigned int size);
    /// <summary>
    /// Get the next message from the server. 
    /// Return true on success
    /// </summary>
    /// <param name="buffer">the received message output</param>
    /// <param name="maxSize">The buffer size</param>
    /// <param name="size">Return the received message size</param>
    /// <returns></returns>
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
    /// <summary>
    /// Reset the communication(Not the channel, for cleaning buffers and similar stuff)
    /// </summary>
    void reset();
private:
#ifdef WIN32
    SOCKET m_socket;
#else
    int m_socket;
#endif //WIN32
    std::string m_serverIp;
    int m_serverPort;
    /// <summary>
    /// Connect to a server
    /// </summary>
    /// <param name="serverIp"></param>
    /// <param name="serverPort"></param>
    /// <returns></returns>
    bool connectToServer(const std::string& serverIp, int serverPort);
};

#endif //SHIELD_TcpCommChannel_H
