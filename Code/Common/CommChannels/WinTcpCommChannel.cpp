#include "WinTcpCommChannel.h"



TCPServer::TCPServer() : listenSocket(INVALID_SOCKET), clientSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed: " << result << std::endl;
    }
}

TCPServer::~TCPServer() {
    closesocket(listenSocket);
    closesocket(clientSocket);
    WSACleanup();
}

bool TCPServer::init(std::string serverIp, int serverPort) {
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIp.c_str());
    serverAddress.sin_port = htons(serverPort);

    if (bind(listenSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }

    // Set the socket to non-blocking mode
    u_long mode = 1;  // 1 to enable non-blocking mode
    if (ioctlsocket(listenSocket, FIONBIO, &mode) != 0) {
        std::cout << "Error setting non-blocking mode: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }

    // Optionally, start the thread that runs the server service
    m_thisThread = std::thread(&TCPServer::runServerService, this);

    return true;
}

bool TCPServer::sendMessage(unsigned char* buffer, unsigned int size) {
    int iSendResult = send(clientSocket, (const char*)buffer, size, 0);
    if (iSendResult == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool TCPServer::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) {
    bool messageReceived = false;
    if (false == m_receivedMessages.isEmpty())
    {
        ReceivedMessage receivedMessage;
        if (true == m_receivedMessages.dequeue(receivedMessage))
        {
            *size = getMin(getMin(receivedMessage.size, SO_MAX_MSG_SIZE), maxSize);
            memcpy(buffer, receivedMessage.buffer, *size);
            messageReceived = true;
        }
    }
    return messageReceived;
}

void TCPServer::reset() {
    
}

void TCPServer::runServerService() {
    fd_set readfds;
    std::vector<SOCKET> clientSockets; // List to keep track of client sockets

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(listenSocket, &readfds);
        int max_sd = listenSocket;

        // Add child sockets to set
        for (const auto& sock : clientSockets) {
            FD_SET(sock, &readfds);
            if (sock > max_sd) {
                max_sd = sock;
            }
        }

        // Wait for an activity on one of the sockets, timeout is NULL, wait indefinitely
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "Select error" << std::endl;
        }

        // If something happened on the listenSocket, then its an incoming connection
        if (FD_ISSET(listenSocket, &readfds)) {
            SOCKET newSocket = accept(listenSocket, NULL, NULL);
            if (newSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            }
            else {
                clientSockets.push_back(newSocket); // Add new socket to array of sockets
                clientSocket = newSocket;
                std::cout << "New connection, socket fd is " << newSocket << std::endl;
            }
        }

        // Else it's some IO operation on some other socket
        for (auto it = clientSockets.begin(); it != clientSockets.end(); ) {
            SOCKET sock = *it;

            if (FD_ISSET(sock, &readfds)) {
                // Check if it was for closing, and also read the incoming message
                ReceivedMessage receivedMessage;
                receivedMessage.size = recv(sock, reinterpret_cast<char*>(receivedMessage.buffer), MAX_MESSAGE_SIZE, 0);

                if (receivedMessage.size == 0) {
                    // Somebody disconnected, get his details and print
                    std::cout << "Host disconnected, socket fd: " << sock << std::endl;
                    closesocket(sock);
                    it = clientSockets.erase(it); // Remove from list of sockets
                }
                else {
                    // Save the message that came in
                    m_receivedMessages.enqueue(receivedMessage);
                    ++it;
                }
            }
            else {
                ++it;
            }
        }
    }
}

unsigned int TCPServer::getMin(unsigned int a, unsigned int b)
{
    return (a > b) ? b : a;
}
   

TCPClient::TCPClient() : m_socket(INVALID_SOCKET) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed: " << result << std::endl;
    }
}

TCPClient::~TCPClient() {
    closesocket(m_socket);
    WSACleanup();
}
bool TCPClient::init(std::string serverIp, int serverPort) {
    m_serverIp = serverIp;
    m_serverPort = serverPort;
    return connectToServer(serverIp, serverPort);
}
bool TCPClient::sendMessage(unsigned char* buffer, unsigned int size){
    int iSendResult = send(m_socket, (const char*)buffer, size, 0);
    if (iSendResult == SOCKET_ERROR) {
        std::cout << "Send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return false;
    }
    return true;
}
bool TCPClient::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) {
    int iResult = recv(m_socket, (char*)buffer, maxSize, 0);
    if (iResult > 0) {
        *size = iResult;
        return true;
    }
    else if (iResult == 0) {
        //std::cout << "Connection closing..." << std::endl;
        return false;
    }
    else {
        //std::cout << "Recv failed with error: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return false;
    }
}
void TCPClient::reset() {
}

bool TCPClient::connectToServer(const std::string& serverIp, int serverPort) {
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIp.c_str());
    serverAddress.sin_port = htons(serverPort);

    if (connect(m_socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cout << "Connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return false;
    }
    return true;
}
