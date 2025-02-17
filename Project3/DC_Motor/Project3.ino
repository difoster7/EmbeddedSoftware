#define ENABLE 5
#define DIRA 3
#define DIRB 4

void setup() {
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(DIRA, HI);
  digitalWrite(DIRB, HI);
  digitalWRite(ENABLE, 255);
  delay(2000);
  digitalWrite(ENABLE, 255 * 0.75);
  delay(2000);
  digitalwrite(ENABLE, 255 * 0.5);
  delay(2000);
  digitalWrite(ENABLE, 255 * 0.25);
  delay(2000);
  digitalwrite(ENABLE, 0);
  delay(2000);
}