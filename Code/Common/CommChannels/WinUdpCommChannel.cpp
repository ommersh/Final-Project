#include "WinUdpCommChannel.h"
#include <iostream>
#include <algorithm>


WinTUdpCommChannel::WinTUdpCommChannel() : localSocket(INVALID_SOCKET) {
    // Initialize Winsock
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "WSAStartup failed: " << res << "\n";
    }
}

WinTUdpCommChannel::~WinTUdpCommChannel() {
    closesocket(localSocket);
    WSACleanup();
}

bool WinTUdpCommChannel::Init(std::string localIp, int localPort, std::string destIp, int destPort) {
    if ((localSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError() << "\n";
        return false;
    }

    // Bind the socket to the local address
    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(localPort);
    if (inet_pton(AF_INET, localIp.c_str(), &localAddr.sin_addr) <= 0) {
        std::cerr << "Invalid local IP Address\n";
        closesocket(localSocket);
        return false;
    }

    if (bind(localSocket, (struct sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(localSocket);
        return false;
    }

    // Set the destination address using inet_pton for better IP address handling
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);
    if (inet_pton(AF_INET, destIp.c_str(), &destAddr.sin_addr) <= 0) {
        std::cerr << "Invalid destination IP Address\n";
        closesocket(localSocket);
        return false;
    }
    return true;
}

bool WinTUdpCommChannel::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) {
    sockaddr_in from;
    int fromSize = sizeof(from);
    int recvSize = recvfrom(localSocket, (char*)buffer, maxSize, 0, (sockaddr*)&from, &fromSize);

    if (recvSize == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            // No message received
            *size = 0;
            return false;
        }
        std::cerr << "recvfrom failed with error: " << WSAGetLastError() << "\n";
        return false;
    }

    *size = recvSize;
    return true;
}

bool WinTUdpCommChannel::sendMessage(unsigned char* buffer, unsigned int size) {
    unsigned int bytesSent = 0; // Tracks the number of bytes sent

    while (bytesSent < size) {
        // Calculate the size of the next chunk
        unsigned int chunkSize = (((MAX_MESSAGE_SIZE) < (size - bytesSent)) ? (MAX_MESSAGE_SIZE) : (size - bytesSent));

        // Send the current chunk
        int sent = sendto(localSocket, (char*)buffer + bytesSent, chunkSize, 0, (sockaddr*)&destAddr, sizeof(destAddr));
        if (sent == SOCKET_ERROR) {
            std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
            return false;
        }

        // Update the total number of bytes sent
        bytesSent += sent;
    }
    return true;

}

void WinTUdpCommChannel::reset()  {
}
