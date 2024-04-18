#include "WinTcpCommChannel.h"


WinTcpCommChannel::WinTcpCommChannel(){
    // Initialize Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error code: " << result << std::endl;
    }
}

WinTcpCommChannel::~WinTcpCommChannel() {
    // Close the listening socket if it has been opened
    if (listenSock != INVALID_SOCKET) {
        closesocket(listenSock);
    }
    // Close the connection socket if it has been opened
    if (connectSock != INVALID_SOCKET) {
        closesocket(connectSock);
    }
    // Clean up Winsock
    WSACleanup();
}

bool WinTcpCommChannel::init(std::string localIp, int localPort, std::string destIp, int destPort) {
    m_localIp = localIp;
    m_localPort = localPort;
    m_destIp = destIp;
    m_destPort = destPort;

    //initClient();

    return initServer();
}

bool WinTcpCommChannel::sendMessage(unsigned char* buffer, unsigned int size) {
    if (connectSock == INVALID_SOCKET) {
        if (initClient() == false)
        {
            return false;
        }
    }

    int bytesSent = 0;
    while (size > 0) {
        bytesSent = send(connectSock, reinterpret_cast<const char*>(buffer), size, 0);
        if (bytesSent == SOCKET_ERROR) {
            initClient();
            std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
            return false;
        }
        buffer += bytesSent;
        size -= bytesSent;
    }

    return true;
}

bool WinTcpCommChannel::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) {
    *size = 0;
    if (buffer == nullptr || maxSize <= 0)
    {
        return false;
    }
    if (listenSock == INVALID_SOCKET) {
        return false;
    }

    // Check for incoming data without blocking
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(listenSock, &readfds);

    timeval timeout;
    timeout.tv_sec = 0;  // Non-blocking check
    timeout.tv_usec = 0;

    int selectResult = select(listenSock + 1, &readfds, NULL, NULL, &timeout);
    if (selectResult == 0) {
        // No data available to read
        *size = 0;
        return false;
    }
    else if (selectResult == SOCKET_ERROR) {
        std::cerr << "select failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }

    // Data is available; now we can call recv
    int bytesRead = recv(listenSock, reinterpret_cast<char*>(buffer), maxSize, 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    else if (bytesRead == 0) {
        std::cerr << "Connection closed by peer.\n";
        return false;
    }

    *size = static_cast<unsigned int>(bytesRead);
    return true;

}

void WinTcpCommChannel::reset()
{

}

bool WinTcpCommChannel::initServer() {
    // Initialize the server address structure
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(m_localPort);
    inet_pton(AF_INET, m_localIp.c_str(), &localAddr.sin_addr);

    // Create the server socket
    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "Error creating server socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    // Bind the server socket
    if (bind(listenSock, (struct sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSock);
        WSACleanup();
        return false;
    }

    // Start listening
    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSock);
        WSACleanup();
        return false;
    }

    return true;
}

bool WinTcpCommChannel::initClient() {
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(m_destPort);
    inet_pton(AF_INET, m_destIp.c_str(), &destAddr.sin_addr);

    // Create the client socket
    connectSock = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSock == INVALID_SOCKET) {
        std::cerr << "Error creating client socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    // Attempt to connect to the server
    if (connect(connectSock, (struct sockaddr*)&destAddr, sizeof(destAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(connectSock);
        WSACleanup();
        return false;
    }

    return true;
}
