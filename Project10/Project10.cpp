#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <unistd.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

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
        unsigned char buffer[1] = { 0x00 };
        wiringPiSPIDataRW(SPI_CHANNEL, buffer, 1);
        printf("Received: 0x%02X\n", buffer[0]);
        sleep(1);
    }
}