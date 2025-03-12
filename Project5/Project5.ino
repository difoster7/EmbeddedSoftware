// Include the driver file
#include "DHTesp.h"

// Declare the driver
DHTesp dht;

int D6 = 12;
float temperature;
float humidity;

void setup() {
    Serial.begin(115200);

    // Initialize the driver
    dht.setup(D6, DHTesp::DHT11);
}

void loop() {

  // Read humidity and temperature into float variables
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  
  // Use printf to write the values formatted to the command line
  Serial.println("Humidity = " + String(humidity));
  Serial.println("Temperature = " + String(temperature));

  // Wait
  delay(3000);

}

