/*
EN.605.715
Project 1
David Foster
*/

String input;
int led = 8;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  Serial.println(("What message would you like to send? Enter 99 to quit."));

  while(Serial.available())
  {
    input = Serial.readString();
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