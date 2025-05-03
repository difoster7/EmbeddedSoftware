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