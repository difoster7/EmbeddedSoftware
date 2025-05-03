#include <SPI.h>

volatile byte sendBuffer[13]; // 1 marker + 3 floats * 4 bytes
volatile byte byteIndex = 0;

void setup() {
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);  // Enable SPI
  SPCR |= _BV(SPIE); // Enable SPI interrupt
  sei();
  prepareData();
}

void prepareData() {
  union {
    float f;
    byte b[4];
  } data;

  // Example floats
  float values[3] = {3.14, 2.718, 1.618};

  sendBuffer[0] = 0xAA; // Start-of-frame marker

  for (int i = 0; i < 3; i++) {
    data.f = values[i];
    for (int j = 0; j < 4; j++) {
      sendBuffer[1 + i * 4 + j] = data.b[j];
    }
  }
}

ISR(SPI_STC_vect) {
  byte dummy = SPDR;
  SPDR = sendBuffer[byteIndex];
  byteIndex = (byteIndex + 1) % 13;
}

void loop() {
  // Optionally update values and call prepareData() again
  delay(1000);
}


/*
#include <SPI.h>

union{
  float f;
  byte b[4];
} data;

volatile int byteToSend;
volatile boolean dataSent = false;
volatile byte sendBuffer[5];

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);  // Enable SPI
  SPCR |= _BV(SPIE); // Enable SPI interrupt
  sei();             // Enable global interrupts
  data.f = 1.234;
  byteToSend = 0;
  sendBuffer[0] = 0xAA; // Start marker
  for (int i = 0; i < 4; i++) {
    sendBuffer[i + 1] = data.b[i];
  }
}

ISR(SPI_STC_vect) {
  Serial.println("Received SPI Interrupt");
  if (byteToSend == 0)
  {
    Serial.println("~~~~~~~~~~~~~");
  }
  SPDR = sendBuffer[byteToSend];
  Serial.print("Sending Byte: ");
  Serial.println(byteToSend);
  Serial.print("Sending ");
  Serial.println(sendBuffer[byteToSend], HEX);
  byteToSend = (byteToSend + 1) % 5;
}

void loop() {
  /*
  if (dataSent) {
    // Optional: Change the byte to send next time
    data.f++;
    Serial.print("data is now ");
    Serial.println(data.f);
    Serial.println(data.b[3]);
    dataSent = false;
  }
}
*/