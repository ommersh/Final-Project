#include "ENetCommChannel.h"



ENetCommChannelServer::ENetCommChannelServer() {
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
    }
}
ENetCommChannelServer::~ENetCommChannelServer() {
    // Cleanup
    enet_host_destroy(server);
    enet_deinitialize();
}

bool ENetCommChannelServer::init(std::string serverIp, int serverPort) {

    enetAddress.host = ENET_HOST_ANY; // Bind the server to all available interfaces
    enetAddress.port = serverPort;
    enet_address_set_host(&enetAddress, serverIp.c_str());

    server = enet_host_create(&enetAddress /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);
    if (server == NULL)
    {
        std::cout << "An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }

    m_thisThread = std::thread(&ENetCommChannelServer::runServerService, this);


    return true;
}

bool ENetCommChannelServer::sendMessage(unsigned char* buffer, unsigned int size) {
    bool messageSent = false;
    if (m_client != nullptr)
    {
        ENetPacket* packet = enet_packet_create(buffer,
            size,
            ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(m_client, 0, packet);
        enet_host_flush(m_client->host);
        messageSent = true;
    }
    return messageSent;
}

bool ENetCommChannelServer::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) {
    bool messageReceived = false;
    if (false == m_receivedMessages.isEmpty())
    {
        ReceivedMessage receivedMessage;
        if (true == m_receivedMessages.dequeue(receivedMessage))
        {
            *size = getMin(getMin(receivedMessage.size, MAX_MESSAGE_SIZE), maxSize);
            memcpy(buffer, receivedMessage.buffer, *size);
            messageReceived = true;
        }
    }
    return messageReceived;
}
void ENetCommChannelServer::reset()
{

}
void ENetCommChannelServer::runServerService()
{
    while (1)
    {
        if (server != nullptr)
        {
            ENetEvent event;
            while (enet_host_service(server, &event, 1000) > 0) { // Wait up to 1000 milliseconds for an event
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    m_client = event.peer;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    if (event.packet->dataLength > 0 && event.packet->data != nullptr)
                    {
                        ReceivedMessage receivedMessage;
                        receivedMessage.size = getMin(event.packet->dataLength, MAX_MESSAGE_SIZE);
                        memcpy(receivedMessage.buffer, event.packet->data, receivedMessage.size);
                        m_receivedMessages.enqueue(receivedMessage);
                    }
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    m_client = nullptr;
                    break;
                }
            }
        }
    }
}






ENetCommChannelClient::ENetCommChannelClient() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
    }
}
ENetCommChannelClient::~ENetCommChannelClient() {
    enet_host_destroy(clientHost);
    enet_deinitialize();
}
bool ENetCommChannelClient::init(std::string serverIp, int serverPort) {
    bool connected = false;
    m_serverIp = serverIp;
    m_serverPort = serverPort;
    connected = connectToServer(m_serverIp, m_serverPort);
    m_thisThread = std::thread(&ENetCommChannelClient::runClientService, this);

    return connected;
}
bool ENetCommChannelClient::sendMessage(unsigned char* buffer, unsigned int size) {
    //check if connected
    if (!serverPeer) {
        //try to connect if not
        if (!connectToServer(m_serverIp, m_serverPort))
            return false;
    }

    // Create an ENet packet with the data to send
    ENetPacket* packet = enet_packet_create(buffer, size, ENET_PACKET_FLAG_RELIABLE);

    // Send the packet to the server peer on channel id 0
    if (enet_peer_send(serverPeer, 0, packet) < 0) {
        std::cerr << "Failed to send packet." << std::endl;
        return false;
    }

    // Flush the ENet host to immediately start sending queued packets
    enet_host_flush(clientHost);
    return true;
}

bool ENetCommChannelClient::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) {
    bool messageReceived = false;
    if (false == m_receivedMessages.isEmpty())
    {
        ReceivedMessage receivedMessage;
        if (true == m_receivedMessages.dequeue(receivedMessage))
        {
            *size = getMin(getMin(receivedMessage.size, MAX_MESSAGE_SIZE), maxSize);
            memcpy(buffer, receivedMessage.buffer, *size);
            messageReceived = true;
        }
    }
    return messageReceived;
}
void ENetCommChannelClient::reset()
{

}

bool ENetCommChannelClient::connectToServer(const std::string& host, enet_uint16 port) {
    clientHost = enet_host_create(nullptr, 1, 2, 0, 0);  // 1 peer, 2 channels
    if (!clientHost) {
        std::cerr << "Failed to create ENet client host." << std::endl;
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    serverPeer = enet_host_connect(clientHost, &address, 2, 0);
    if (!serverPeer) {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        return false;
    }

    // Wait up to 5 seconds for the connection attempt to succeed
    ENetEvent event;
    if (enet_host_service(clientHost, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << host << ":" << port << " succeeded." << std::endl;
        return true;
    }
    else {
        enet_peer_reset(serverPeer);
        std::cerr << "Connection failed." << std::endl;
        return false;
    }
}

void ENetCommChannelClient::runClientService()
{
    while (1)
    {
        if (clientHost != nullptr)
        {
            ENetEvent event;
            while (enet_host_service(clientHost, &event, 1000) > 0) { // Wait up to 1000 milliseconds for an event
                switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    if (event.packet->dataLength > 0 && event.packet->data != nullptr)
                    {
                        ReceivedMessage receivedMessage;
                        receivedMessage.size = getMin(event.packet->dataLength, MAX_MESSAGE_SIZE);
                        memcpy(receivedMessage.buffer, event.packet->data, receivedMessage.size);
                        m_receivedMessages.enqueue(receivedMessage);
                    }
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server." << std::endl;
                    serverPeer = nullptr;
                    break;
                }
            }
        }
    }
}

unsigned int ENetCommChannelClient::getMin(unsigned int a, unsigned int b)
{
    return (a > b) ? b : a;
}
unsigned int ENetCommChannelServer::getMin(unsigned int a, unsigned int b)
{
    return (a > b) ? b : a;
}