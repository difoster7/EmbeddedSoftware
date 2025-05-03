#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000

typedef struct {
    float roll;
    float pitch;
    float yaw;
} rpy;

std::queue<rpy> dataQueue;

void *handleTCP(void *args)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printf("Failed to create server\n");
        pthread_exit(NULL);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accept any IP
    serverAddr.sin_port = htons(54000);       // Port to listen on

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);
    printf("listening for connection.\n");

    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

    // Connection is made, now clear queue so we start sending with the most recent data
    std::queue<rpy> empty;
    std::swap(dataQueue, empty);

    while(1)    // send data forever
    {
        while(!dataQueue.empty())
        {
            rpy outData = dataQueue.front();
            dataQueue.pop();
            send(clientSocket, &outData, sizeof(outData), 0);
        }
    }
    pthread_exit(NULL);
} // end handleTCP

int main(void) {
    pthread_t tcpThread;
    int tcpThreadID = 1;
    pthread_create(&tcpThread, NULL, handleTCP, (void *)&tcpThreadID);


    if (wiringPiSetup() == -1) return 1;
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) return 1;

    unsigned char byte;
    unsigned char floatBytes[12]; // 3 floats
    rpy data;

    while (1) {
        // Find start-of-frame marker (0xAA)
        do {
            byte = 0x00;
            wiringPiSPIDataRW(SPI_CHANNEL, &byte, 1);
        } while (byte != 0xAA);

        // Read next 12 bytes (3 floats)
        for (int i = 0; i < 12; i++) {
            floatBytes[i] = 0x00;
            wiringPiSPIDataRW(SPI_CHANNEL, &floatBytes[i], 1);
        }

        // Reconstruct floats using a union
        union {
            unsigned char b[4];
            float f;
        } u;   

        printf("Received floats: ");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                u.b[j] = floatBytes[i * 4 + j];
            }
            if (i == 0) {
                data.roll = u.f;
            } else if (i == 1) {
                data.pitch = u.f;
            } else {
                data.yaw = u.f;
            }
            printf("%.5f ", u.f);
        }
        printf("\n");

        dataQueue.push(data);

        usleep(100000);
    }

    return 0;
} //end main