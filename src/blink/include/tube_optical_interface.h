#ifndef TUBE_OPTICAL_INTERFACE_H
#define TUBE_OPTICAL_INTERFACE_H

#include <stdint.h>

// Initialize the SPI pins for ADCs (not spi itself)
void tube_optical_ctrl_init(void);

// Read a raw 12-bit or 16-bit value from a specific ADC and Channel
// adc_index: 0 or 1
// channel: 0 through 4
uint16_t hw_adc_read_raw(int adc_index, int channel);

#endif