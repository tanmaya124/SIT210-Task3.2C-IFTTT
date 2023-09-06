#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;  // Create an instance of the BH1750 light sensor library

const long interval = 1000;  // Set the time interval for reading light levels (in milliseconds)
unsigned long previousMillis = 0;  // Initialize a variable to keep track of the last time we checked

float light_level;  // Store the current light level
const float LIGHT_THRESHOLD = 1000;  // Define the threshold for considering it as "sunlight"

void setup() {
  // Initialize serial communication and wait for the serial port to open
  Serial.begin(9600);

  // Initialize the I2C bus for communication with sensors
  Wire.begin();

  // Initialize the BH1750 light sensor
  lightMeter.begin();

  // Initialize properties (settings) defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Set the debug message level for more detailed information (optional)
  setDebugMessageLevel(2);
}

void loop() {
  // Update IoT Cloud to communicate with the Arduino Cloud
  ArduinoCloud.update();
  
  unsigned long currentMillis = millis();  // Get the current time in milliseconds
  
  // Check if it's time to read the light level based on the defined interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Record the current time as the last checked time
    
    // Read the light level from the sensor
    light_level = lightMeter.readLightLevel();
    
    // Check if the light level is above the threshold and if we haven't already updated
    if (light_level >= LIGHT_THRESHOLD && sunlight_update != "started")
    {
      sunlight_update = "started";  // Indicate that sunlight has started
    }
    // Check if the light level is below the threshold and if we haven't already updated
    else if (light_level < LIGHT_THRESHOLD && sunlight_update != "stopped") {
      sunlight_update = "stopped";  // Indicate that sunlight has stopped
    }
  }
}
