#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

const long interval = 1000;
unsigned long previousMillis = 0;

float light_level;
const float LIGHT_THRESHOLD = 1000;

void setup() {
  // Initialize serial and wait for port to open
  Serial.begin(9600);

  // Initialize the I2C bus
  Wire.begin();

  // Initialize the light sensor
  lightMeter.begin();

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Set debug message level
  setDebugMessageLevel(2);
}

void loop() {
  // Update IoT Cloud
  ArduinoCloud.update();
  
  unsigned long currentMillis = millis();
  
  // Check if it's time to read the light level
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read the light level from the sensor
    light_level = lightMeter.readLightLevel();
    
    // Check if the light level is above the threshold
    if (light_level >= LIGHT_THRESHOLD && sunlight_update != "started")
    {
      sunlight_update = "started";
    }
    // Check if the light level is below the threshold
    else if (light_level < LIGHT_THRESHOLD && sunlight_update != "stopped") {
      sunlight_update = "stopped";
    }
  }
}
