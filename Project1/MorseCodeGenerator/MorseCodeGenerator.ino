/*
EN.605.715
Project 1
David Foster
*/

#include <avr/wdt.h>

String input;
String morse;
int led = 8;
int timeUnit = 100;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  delay(2000);
  Serial.println("What message would you like to send? Enter 99 to quit.");
}

void loop() {
  while(Serial.available() > 0)
  {
    input = Serial.readString();
    input.toUpperCase();

    if (input.length() == 3 && input[0] == '9' && input[1] == '9')
    {
      reboot();
    }

    Serial.println("Now encoding: " + input);
    for (int i = 0; i < input.length(); i++)
    {
      if (input[i] != ' ')
      {
        morse = convertToMorse(input[i]);
        // Serial.println(morse);
        for (int j = 0; j < morse.length(); j++)
        {
          if (morse[j] == '.')
          {
            digitalWrite(led, HIGH);
            delay(timeUnit);
            digitalWrite(led, LOW);
          }
          
          if (morse[j] == '-')
          {
            digitalWrite(led, HIGH);
            delay(timeUnit * 3);
            digitalWrite(led, LOW);
          }

          if (j < morse.length() - 1) // If it's not the last dot/dash, add a pause before the next character
          {
            delay(timeUnit);
          }
        }
        delay(timeUnit * 3);  // delay 3 time units at end of character
      } else {
        // If it's not the last word, add a pause before the next word
        delay(4 * timeUnit);  // wait for an additional 4 time units at the end of a word
      }
    }
  }
}

String convertToMorse(char input_char)
{
  switch(input_char)
  {
    case 'A': 
      return String(".-");
      break;
    case 'B':
      return String("-...");
      break;
    case 'C':
      return String("-.-.");
      break;
    case 'D':
      return String("-..");
      break;      
    case 'E':
      return String(".");
      break;
    case 'F':
      return String("..-.");
      break;
    case 'G':
      return String("--.");
      break;
    case 'H':
      return String("....");
      break;
    case 'I':
      return String("..");
      break;
    case 'J':
      return String(".---");
      break;
    case 'K':
      return String("-.-");
      break;
    case 'L':
      return String(".-..");
      break;
    case 'M':
      return String("--");
      break;
    case 'N':
      return String("-.");
      break;
    case 'O':
      return String("---");
      break;
    case 'P':
      return String(".--.");
      break;
    case 'Q':
      return String("--.-");
      break;
    case 'R':
      return String(".-.");
      break;
    case 'S':
      return String("...");
      break;
    case 'T':
      return String("-");
      break;
    case 'U':
      return String("..-");
      break;
    case 'V':
      return String("...-");
      break;
    case 'W':
      return String("..-");
      break;
    case 'X':
      return String("-..x");
      break;
    case 'Y':
      return String("-.--");
      break;
    case 'Z':
      return String("--..");
      break;
    case '1':
      return String(".----");
      break;
    case '2':
      return String("..---");
      break;
    case '3':
      return String("...--");
      break;
    case '4':
      return String("....-");
      break;
    case '5':
      return String(".....");
      break;
    case '6':
      return String("-....");
      break;
    case '7':
      return String("--...");
      break;
    case '8':
      return String("---..");
      break;
    case '9':
      return String("----.");
      break;
    case '0':
      return String("-----");
      break;
    default:
      return String();
  }
}

// reboot code taken from user dmjlambert at https://forum.arduino.cc/t/soft-reset-and-arduino/367284/5
void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}