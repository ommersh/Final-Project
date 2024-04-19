#ifndef SHIELD_ENetCommChannel_H    // Check if the symbol SHIELD_ENetCommChannel_H is not defined
#define SHIELD_ENetCommChannel_H    // Define the symbol SHIELD_ENetCommChannel_H
#include "ICommChannel.h"
#include <iostream>
#include <enet/enet.h>
#include <thread>
#include "SafeQueue.h"


class ENetCommChannelServer : public ICommChannel
{
public:
    ENetCommChannelServer();
    ~ENetCommChannelServer();
    bool init(std::string serverIp, int serverPort);
    bool sendMessage(unsigned char* buffer, unsigned int size) override;
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
    void reset();
private:
    ENetAddress enetAddress;
    ENetHost* server;
    ENetPeer* m_client;
    std::thread m_thisThread;

    SafeQueue< ReceivedMessage> m_receivedMessages;
    void runServerService();
    unsigned int getMin(unsigned int a, unsigned int b);

};

class ENetCommChannelClient : public ICommChannel
{
public:
    ENetCommChannelClient();
    ~ENetCommChannelClient();
    bool init(std::string serverIp, int serverPort);
    bool sendMessage(unsigned char* buffer, unsigned int size) override;
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
    void reset() override;
private:
    ENetHost* clientHost;
    ENetPeer* serverPeer;  // Peer representing the server connection
    std::string m_serverIp;
    int m_serverPort;
    bool connectToServer(const std::string& host, enet_uint16 port);
    SafeQueue< ReceivedMessage> m_receivedMessages;
    void runClientService();
    std::thread m_thisThread;
    unsigned int getMin(unsigned int a, unsigned int b);


};


#endif //SHIELD_ENetCommChannel_H
