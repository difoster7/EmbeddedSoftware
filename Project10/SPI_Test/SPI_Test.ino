#include <SPI.h>

volatile byte dataToSend = 0x42; // Example byte
volatile boolean dataSent = false;

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);  // Enable SPI
  SPCR |= _BV(SPIE); // Enable SPI interrupt
  sei();             // Enable global interrupts
}

ISR(SPI_STC_vect) {
  Serial.println("Received SPI Interrupt");
  byte received = SPDR;  // Read byte from master
  SPDR = dataToSend;     // Load data to send next
  dataSent = true;
}

void loop() {
  if (dataSent) {
    // Optional: Change the byte to send next time
    dataToSend++;
    dataSent = false;
  }
}
