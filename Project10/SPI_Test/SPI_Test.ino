#include <SPI.h>

void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Set SPI speed to 1MHz for level limiter
}

void loop() {
  // put your main code here, to run repeatedly:
  byte outgoing = 0xFA;
  SPI.transfer(outgoing);
}
