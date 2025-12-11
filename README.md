# Weather Station Texas Instruments OPT3001 Arduino Library

Arduino library for interfacing with the Texas Instruments OPT300x ambient light sensor.

## Description

The OPT300x is an ambient light sensor that measures illuminance in lux with a wide dynamic range from 0.01 lux to 83,000 lux. The sensor features automatic full-scale range selection, allowing it to automatically optimize the measurement range for current light conditions. This library provides a simple interface to read illuminance measurements via I2C.

## Installation

### Arduino IDE

Install manually:
1. Download or clone this repository
2. Place it in your Arduino `libraries` folder
3. Restart the Arduino IDE

### PlatformIO

Add the library manually to your `platformio.ini` file:

```ini
lib_deps = 
   https://github.com/Rthrall/OPT300X.git
```

## Hardware Connections

Connect the OPT3001 to your Arduino using I2C:

- VCC → 3.3V (check your board's specifications)
- GND → GND
- SDA → SDA (I2C Data)
- SCL → SCL (I2C Clock)
- ADDR → GND or VCC (I2C address selection)
- INT → NC (unused by this library)

The I2C address is determined by the ADDR pin:
- ADDR = GND: 0x44
- ADDR = VCC: 0x45
- ADDR = floating: 0x46 or 0x47 (depends on package)

## Usage

### Continuous Conversion Mode

```cpp
#include <Wire.h>
#include <opt300x.h>

// Create sensor object
opt3001 lux300x;

// I2C address (0x44, 0x45, 0x46, or 0x47)
const uint8_t I2C_ADDRESS = 0x44;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C
  Wire.begin();
  
  // Setup the OPT3001 (I2C library, I2C address)
  if (lux300x.setup(Wire, I2C_ADDRESS) != 0) {
    Serial.println("Failed to setup OPT300x");
    return;
  }
  
  // Detect the sensor
  if (lux300x.detect() != 0) {
    Serial.println("OPT300x not detected");
    return;
  }
  
  // Configure conversion time (100ms or 800ms)
  lux300x.config_set(OPT300x_CONVERSION_TIME_800MS);
  
  // Enable continuous conversion mode
  lux300x.conversion_continuous_enable();
  
  Serial.println("OPT300x initialized");
}

void loop() {
  int lux;
  
  // Read illuminance in lux
  if (lux300x.isready() == 0) {
    lux = lux300x.get_lux
    Serial.print("Illuminance: ");
    Serial.print(lux);
    Serial.println(" lux");
  } else {
    Serial.println("Failed to read illuminance");
  }
  
  delay(1000);
}
```
## API Reference

### setup(TwoWire &i2c_library, uint8_t i2c_address)

Initializes the OPT300x sensor.

- `i2c_library`: I2C library instance to use (typically `Wire`)
- `i2c_address`: I2C address (0x44, 0x45, 0x46, or 0x47)

Returns 0 on success, or -EINVAL if the I2C address format is invalid.

### detect(void)

Detects and verifies the presence of an OPT3001 sensor by reading the manufacturer ID and device ID.

Returns 0 if a valid OPT300x device is detected, or a negative error code otherwise.

### config_set(opt300x_conversion_time ct)

Configures the sensor conversion time and enables automatic full-scale range selection.

- `ct`: Conversion time setting (`OPT3001_CONVERSION_TIME_100MS` or `OPT3001_CONVERSION_TIME_800MS`)

Returns 0 on success, or a negative error code on I2C communication failure.

### conversion_continuous_enable(void)

Enables continuous conversion mode. The sensor will continuously perform conversions and update the result register.

Returns 0 on success, or a negative error code on I2C communication failure.

### conversion_continuous_disable(void)

Disables continuous conversion mode and places the sensor in low-power shutdown state.

Returns 0 on success, or a negative error code on I2C communication failure.

### conversion_singleshot_trigger(void)

Triggers a single-shot conversion. After the conversion completes, the sensor automatically returns to shutdown mode.

Returns 0 on success, or a negative error code on I2C communication failure.

### isready()
Checks if device is ready to send data

Returns 0 on success, or a negative error code on I2C communication failure.

**Note:** Ensure a conversion has completed before calling this function. In single-shot mode, wait for the conversion time after triggering. In continuous mode, the result is updated automatically.

### get_lux()
Reads the current illuminance measurement in lux.
## Specifications

- Measurement range: 0.01 lux to 83,000 lux
- Conversion time: 100ms or 800ms (configurable)
- Automatic full-scale range selection
- I2C addresses: 0x44, 0x45, 0x46, or 0x47
- Operating modes: Continuous conversion, single-shot conversion, shutdown
- Resolution: 0.01 lux (with automatic range selection)
