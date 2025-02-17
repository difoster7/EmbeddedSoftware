#include <RingBuf.h>

#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define IR 2
int count = 0; // Interrupt counter
int speed = 255;
int oldSpeed = 0;
int oldMillis = 0;
int newMillis = 0;
int diffMillis = 0;

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

  // Use the noise on A0 to randomize the seed
  randomSeed(analogRead(0));

  // Setup motor pins
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);

  // setup IR detector
  pinMode(IR, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), tach, FALLING);
}

void loop() {
  if (speed != oldSpeed)
  {
    oldSpeed = speed;
    Serial.println("Speed changed to: " + String(oldSpeed));
    analogWrite(ENABLE, oldSpeed);
  }

  // send last tach time
  if (!dataBuff.isEmpty())
  {
    int tach;
    dataBuff.lockedPop(tach);
    Serial.println(tach);
  }
}

//IR interrupt
void tach() {
  newMillis = millis();
  diffMillis = newMillis - oldMillis;
  if (diffMillis <= 3)  // prevent double triggers
  {
    return;
  }
  dataBuff.lockedPush(diffMillis);
  oldMillis = newMillis;
}


// Timer1 interrupt, triggers every second
ISR(TIMER1_COMPA_vect) {
  if(count == 0)
  {
    speed = random(255*0.5, 256); // Randomly change speed between 100% and 50% duty cycle
    Serial.println("Changing speed to: " + String(speed));
  }

  count++;
  if(count > 4) {
    count = 0;
  } 
}