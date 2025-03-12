#include "DHTesp.h"
#include <Wire.h>  
#include "SSD1306Wire.h"

DHTesp dht;

int D6 = 12;
float temperature;
float humidity;

int D3 = 0;
int D4 = 2;

SSD1306Wire  display(0x3c, D3, D4);

void setup() {
    Serial.begin(115200);

  dht.setup(D6, DHTesp::DHT11);
  display.init();

  display.setFont(ArialMT_Plain_16);

  display.setColor(WHITE);
}

void loop() {

  // Read humidity and temperature into float variables
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  
  // Use printf to write the values formatted to the command line
  Serial.println("Humidity = " + String(humidity));
  Serial.println("Temperature = " + String(temperature));

  display.clear();
  display.drawString(0, 0, "Humidity = " + String(humidity));
  display.drawString(0, 32, "Temp = " + String(temperature) + "c");
  display.display();

  // Wait
  delay(3000);

}

