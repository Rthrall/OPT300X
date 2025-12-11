#include <Wire.h>
#include <opt300x.h>

// Create lux object
opt300x lux300x;

// I2C address (0x44, 0x45, 0x46, or 0x47)
const uint8_t I2C_ADDRESS = 0x44;

void setup() {
  Serial.begin(9600);
  
  // Initialize I2C
  Wire.begin();
  
  // Setup the OPT300X (I2C library, I2C address)
  if (lux300x.setup(Wire, I2C_ADDRESS) != 0) {
    Serial.println("Failed to setup OPT300X");
    return;
  }
  
  // Detect the lux
  if (lux300x.detect() != 0) {
    Serial.println("OPT300X not detected");
    return;
  }
  
  // Configure conversion time (100ms or 800ms)
  lux300x.config_set(OPT300X_CONVERSION_TIME_800MS);
  
  // Enable continuous conversion mode
  lux300x.conversion_continuous_enable();
  
  Serial.println("OPT300X initialized");
}

void loop() {
  float lux;
  
  // Read illuminance in lux
  if (lux300x.lux_read(&lux) == 0) {
    Serial.print("Illuminance: ");
    Serial.print(lux);
    Serial.println(" lux");
  } else {
    Serial.println("Failed to read illuminance");
  }
  
  delay(1000);
}
