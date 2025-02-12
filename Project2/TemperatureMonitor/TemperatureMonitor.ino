int tempPin = 0;

const uint8_t
  pinTick = 10,
  pinLED = LED_BUILTIN;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  /*
  // Initialize timer settings
  cli();                  // Disable interrupts
  TCCR1A = 0;			        // Reset entire TCCR1A register
  TCCR1B = 0;			        // Reset entire TCCR1B register
  TCCR1A |= B00000100;		// Set CS12 to 1 so we get Prescalar = 1024
  TIMSK1 |= B00000010;    // Set OCIE1A to 1 so we enable compare match A
  OCR1A = 15625;          // Set compare match to 15625 for 1 second interrupts
  sei();                  // Enable interrupts
*/

  pinMode(pinTick, OUTPUT);  //connected to OC1B; pulses at 1pps
  pinMode(pinLED, OUTPUT);   //toggled in OC1A interrupt at 1-sec intervals

  //timer 1 setup
  //  WGM15, prescaler /256
  //  connect OC1B to pinTick
  //  enable the OC1A interrupt
  TCCR1A = _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);
  TIMSK1 = _BV(OCIE1A);

  //with a /256 prescaler, timer 1 ticks at 62.5kHz
  //1-second is thus 62500 ticks or 0xF424
  OCR1A = 0xF424;
  //defines width of 1pps on pinTick
  //  width == N * 16uS
  //      in this example, should be ~160uS
  OCR1B = 0x0064;
}

void loop() {
  Serial.println("Hello");
}

ISR(TIMER1_COMPA_vect) {
  static bool
    flag = false;

  //on each interrupt, toggle the state of the on-board LED
  flag ^= true;
  digitalWrite(pinLED, flag);
  /*
  Serial.println("Interrupt");
  TCNT1  = 0; //First, set the timer back to 0 so it resets for next interrupt
  
  // Temperature read and conversion code from Elegoo tutorial
  int tempReading = analogRead(tempPin);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit

  Serial.println("Temp F is: " + String(tempF));
  */
}
