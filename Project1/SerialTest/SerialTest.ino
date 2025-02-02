String input;
int led = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  while(Serial.available())
  {
    //input = "";
    input = Serial.readString();
    Serial.println(input);
    if (input[0] == '1')
    {
      digitalWrite(led, HIGH);
    } else
    {
      digitalWrite(led, LOW);
    }

  }
}
