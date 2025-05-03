#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void TaskRPYRead(void *pvParameters);
void TaskSerialSend(void *pvParameters);

QueueHandle_t orientationQueue; // global queue handler

float RPYReadArray[3] = {0, 0, 0};
float RPYSendArray[3] = {0, 0, 0};

void setup() {

  Serial.begin(9600);
    while(!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  bno.setExtCrystalUse(true);

  // Create queue
  orientationQueue = xQueueCreate(10, sizeof(float)*3);

  if(orientationQueue != NULL)
  {
    // Create task to read Roll, Pitch, and Yaw from the BNO055 sensor
    xTaskCreate(TaskRPYRead,// Task function
            "RPYRead",// Task name
            128,// Stack size 
            NULL,
            1,// Priority
            NULL);
    // Create task to send data over serial port

    xTaskCreate(TaskSerialSend,// Task function
            "SerialSend",// Task name
            128,// Stack size 
            NULL,
            2,// Priority
            NULL);
  }
}

void loop() {
  // empty with FreeRTOS
}

void TaskRPYRead(void *pvParameters)
{
  sensors_event_t event; 
  for(;;)
  {
    bno.getEvent(&event);

    RPYReadArray[0] = event.orientation.z;
    RPYReadArray[1] = event.orientation.y;
    RPYReadArray[2] = event.orientation.x;

    xQueueSend(orientationQueue, &RPYReadArray, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskSerialSend(void *pvParameters)
{
  for(;;)
  {
    if(xQueueReceive(orientationQueue, &RPYSendArray, portMAX_DELAY) == pdPASS)
    {
      Serial.print("Roll: ");
      Serial.print(RPYSendArray[0], 4);
      Serial.print("\tPitch: ");
      Serial.print(RPYSendArray[1], 4);
      Serial.print("\tYaw: ");
      Serial.print(RPYSendArray[2], 4);
      Serial.println("");
    }
  }
}
