#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);

byte dataBuffer[2][13];
volatile byte sendBuffer[13];
volatile byte byteIndex = 0;
int validBuffer = 0;

 
void setup(void) 
{
  Serial.begin(9600);
 
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);  // Enable SPI
  SPCR |= _BV(SPIE); // Enable SPI interrupt
  sei();

}

void prepareData(float inData[]) {
  union {
    float f;
    byte b[4];
  } dataConvert;

  dataBuffer[validBuffer^1][0] = 0xAA; // Start-of-frame marker

  for (int i = 0; i < 3; i++) {
    dataConvert.f = inData[i];
    for (int j = 0; j < 4; j++) {
      dataBuffer[validBuffer^1][1 + i * 4 + j] = dataConvert.b[j];
    }
  }

  validBuffer = validBuffer^1;
}

ISR(SPI_STC_vect) {
  byte dummy = SPDR;
  if (byteIndex == 0) // Safe to switch data
  {
    for (int i = 0; i < 13; i++)
    {
      sendBuffer[i] = dataBuffer[validBuffer][i];
    }
  }
  SPDR = sendBuffer[byteIndex];
  byteIndex = (byteIndex + 1) % 13;
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("Roll: ");
  Serial.print(event.orientation.z, 4);
  Serial.print("\tPitch: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tYaw: ");
  Serial.print(event.orientation.x, 4);
  Serial.println("");

  float newData[3] = {event.orientation.z, event.orientation.y, event.orientation.x};
  prepareData(newData);
}
