#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define RPI_IP "192.168.1.34"

typedef struct {
    float roll;
    float pitch;
    float yaw;
} rpy;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = inet_addr(RPI_IP);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        return -1;
    }

    while (1)   // Continuously receive and print data
    {
        rpy orient = { 0, 0, 0};
        recv(sock, (char*)&orient, sizeof(orient), 0);
        printf("Roll: %0.2f\tPitch: %0.2f\tYaw: %0.2f\n", orient.roll, orient.pitch, orient.yaw);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
