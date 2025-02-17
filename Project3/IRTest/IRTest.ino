int irPin = 2; // IR detector
int airPin = A5;
int val = 0;  // variable to store the value read
int aval = 0;

void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode(irPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), tach, FALLING); 
}

void loop() {
  aval = analogRead(airPin); 
  val = digitalRead(irPin);
  Serial.println("analog: " + String(aval));
  Serial.println("digital: " + String(val));
  delay(1000);
}

void tach()
{
  Serial.println("Interrupt trigger.");
}