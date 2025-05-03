#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000

int main(void) {
    if (wiringPiSetup() == -1) return 1;
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) return 1;

    unsigned char byte;
    unsigned char floatBytes[12]; // 3 floats

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
            printf("%.5f ", u.f);
        }
        printf("\n");

        sleep(1);
    }

    return 0;
}



/*
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <unistd.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000

union floatSPI{
    float f;
    unsigned char b[4];
};

int main(void)
{

    if (wiringPiSetup() == -1)
    {
        printf("WiringPi initialization failed!\n");
        return -1;
    }

    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        printf("SPI setup failed!\n");
        return -1;
    }

    while(1)
    {
        unsigned char byte;
        do {
            wiringPiSPIDataRW(SPI_CHANNEL, &byte, 1);
        } while(byte != 0xAA);

        floatSPI foo;
        wiringPiSPIDataRW(SPI_CHANNEL, &foo.b[0], 1);
        printf("0x%02X", foo.b[0]);
        usleep(5000);
        wiringPiSPIDataRW(SPI_CHANNEL, &foo.b[1], 1);
        printf(" %02X", foo.b[1]);
        usleep(5000);
        wiringPiSPIDataRW(SPI_CHANNEL, &foo.b[2], 1);
        printf(" %02X", foo.b[2]);
        usleep(5000);
        wiringPiSPIDataRW(SPI_CHANNEL, &foo.b[3], 1);
        printf(" %02X\n", foo.b[3]);
        printf("Received: %f\n", foo.f);
        sleep(1);
    }
}
    */