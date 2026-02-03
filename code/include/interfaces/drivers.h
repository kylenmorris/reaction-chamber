#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdbool.h>
#include <stdint.h>

// Gets time in ms since boot
uint32_t get_current_time(void);

// Read raw button state; returns 1 if pressed
int hw_button_get_raw(int button_index);

// Initialize SPI, I2C, libraries, etc
void board_setup(void);

// Turn the heater on or off
void hw_heater_toggle(bool enable);

// Set the LED state
void hw_led_set(bool led_on);

// Read data from one sensor
float hw_read_temperature_sensor(int sensor_id);

// Read a raw 12-bit or 16-bit value from a specific ADC and Channel
// adc_index: 0 or 1
// channel: 0 through 4
uint16_t hw_adc_read_raw(int adc_index, int channel);

// Returns a bitmask where bit 0 is Tube 1, bit 1 is Tube 2, etc.
uint16_t hw_tube_sens_read_all(void);


#endif