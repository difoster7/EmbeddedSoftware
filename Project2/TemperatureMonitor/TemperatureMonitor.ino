#include <RingBuf.h>

const int tempPin = 0;  // Recording temperature on pin A0
int count = 0;  // Interrupt counter
RingBuf<int, 100> dataBuff; // Initialize data buffer

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  
  // Initialize timer settings
  TCCR1A = 0;			        // Reset entire TCCR1A register
  TCCR1B = 0;			        // Reset entire TCCR1B register
  TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12);  // Set TCCR1B for 1024 prescalar and clear on match
  TIMSK1 = _BV(OCIE1A); // Enable compare match A
  OCR1A = 15625;          // Set compare match to 15625 for 1 second interrupts
}

void loop() {

  while (!dataBuff.isEmpty())
  {
    int tempReading;
    dataBuff.lockedPop(tempReading);

    // Thermocouple read/conversion code from Elegoo tutorial
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
    float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
    float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    Serial.println("Temp F is: " + String(tempF));
  }

}

// Timer1 interrupt, triggers every second
ISR(TIMER1_COMPA_vect) {
  if(count == 0)
  {
    dataBuff.lockedPush(analogRead(tempPin));
  }

  count++;
  if(count > 9) {
    count = 0;
  } 
}
