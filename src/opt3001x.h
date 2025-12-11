#ifndef OPT300X_H
#define OPT300X_H

/* Arduino libraries */
#include <Arduino.h>
#include <Wire.h>

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/**
 * OPT300x register addresses
 */
enum opt300x_register {
    OPT300x_REGISTER_RESULT = 0x00,
    OPT300x_REGISTER_CONFIG = 0x01,
    OPT300x_REGISTER_LIMITL = 0x02,
    OPT300x_REGISTER_LIMITH = 0x03,
    OPT300x_REGISTER_MANUID = 0x7E,
    OPT300x_REGISTER_DEVIID = 0x7F,
};

/**
 * OPT300x conversion time options
 */
enum opt300x_conversion_time {
    OPT300x_CONVERSION_TIME_100MS,  ///< 100ms conversion time
    OPT300x_CONVERSION_TIME_800MS,  ///< 800ms conversion time
};

/**
 * OPT300x ambient light sensor driver class
 *
 * This class provides an interface to communicate with the Texas Instruments
 * OPT300x ambient light sensor via I2C. The sensor measures illuminance in lux
 * with a wide dynamic range and automatic full-scale setting.
 */
class opt300x {
   public:
    /**
     * Initialize the OPT300x sensor with I2C communication parameters
     * @param[in] i2c_library Reference to the TwoWire I2C library instance to use
     * @param[in] i2c_address I2C address of the OPT300x sensor (must match 0b01000100 pattern)
     * @return 0 on success, negative error code on failure (-EINVAL for invalid address)
     */
    int setup(TwoWire &i2c_library, const uint8_t i2c_address);

    /**
     * Detect and verify the presence of an OPT300x sensor
     * Reads and validates the manufacturer ID (0x5449) and device ID (0x3001)
     * @return 0 if a valid OPT300x device is detected, negative error code otherwise
     */
    int detect(void);

    /**
     * Configure the sensor conversion time and enable automatic full-scale range
     * @param[in] ct Conversion time setting (100ms or 800ms)
     * @return 0 on success, negative error code on I2C communication failure
     */
    int config_set(const enum opt300x_conversion_time ct);

    /**
     * Enable continuous conversion mode
     * The sensor will continuously perform conversions and update the result register
     * @return 0 on success, negative error code on I2C communication failure
     */
    int conversion_continuous_enable(void);

    /**
     * Disable continuous conversion mode (enter shutdown mode)
     * Places the sensor in low-power shutdown state
     * @return 0 on success, negative error code on I2C communication failure
     */
    int conversion_continuous_disable(void);

    /**
     * Trigger a single-shot conversion
     * Initiates one conversion cycle and returns the sensor to shutdown mode when complete
     * @return 0 on success, negative error code on I2C communication failure
     */
    int conversion_singleshot_trigger(void);

    /**
     * Read a 16-bit register from the OPT300x sensor
     * @param[in] reg_address Register address to read from
     * @param[out] reg_content Pointer to variable that will receive the register value
     * @return 0 on success, negative error code on failure (-EINVAL if not initialized, -EIO on I2C error)
     */
    int register_read(const enum opt300x_register reg_address, uint16_t *const reg_content);

    /**
     * Write a 16-bit value to an OPT300x register
     * @param[in] reg_address Register address to write to
     * @param[in] reg_content 16-bit value to write to the register
     * @return 0 on success, negative error code on failure (-EINVAL if not initialized, -EIO on I2C error)
     */
    int register_write(const enum opt300x_register reg_address, const uint16_t reg_content);

    /**
     * Read the current illuminance measurement in lux
     * Reads the result register and converts the raw sensor data to lux using the
     * mantissa and exponent format specified in the OPT300x datasheet
     * @param[out] lux Pointer to float variable that will receive the illuminance value in lux
     * @return 0 on success, negative error code on I2C communication failure
     */
    int lux_read(float *const lux);

   protected:
    TwoWire *m_i2c_library = NULL;
    uint8_t m_i2c_address;
};

#endif
