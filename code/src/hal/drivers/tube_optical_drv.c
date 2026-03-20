#include "drivers.h"

#ifdef USE_HW_OPTICAL

#include <hardware/spi.h>
#include <hardware/gpio.h>

uint16_t read_mcp3208(uint8_t channel, int adc_csn_pin) {
    // Ensure we only use valid channels (0-7)
    if (channel > 7) return 0;

    uint8_t tx_buf[3];
    uint8_t rx_buf[3];

    // Byte 0: 0 0 0 0 0 Start(1) SGL(1) D2
    tx_buf[0] = 0x06 | ((channel >> 2) & 0x01); 
    
    // Byte 1: D1 D0 0 0 0 0 0 0
    tx_buf[1] = (channel & 0x03) << 6;          
    
    // Byte 2: Dummy byte to clock out the remaining data
    tx_buf[2] = 0x00;                           

    // Pull Chip Select low to initiate communication
    gpio_put(adc_csn_pin, 0);

    // Write the 3 bytes while simultaneously reading the response
    spi_write_read_blocking(SPI_INSTANCE_ADC, tx_buf, rx_buf, 3);

    // Push Chip Select high to end communication
    gpio_put(adc_csn_pin, 1);

    // Construct the 12-bit result from the response bytes
    // The first RX byte is ignored. 
    // The lower 4 bits of the second RX byte contain the upper 4 bits of the result.
    // The third RX byte contains the lower 8 bits of the result.
    uint16_t result = ((rx_buf[1] & 0x0F) << 8) | rx_buf[2];
    // printf("RAW BYTES: %02X %02X %02X from channel %d pin %d\n", rx_buf[0], rx_buf[1], rx_buf[2], channel, adc_csn_pin);

    return result;
}

uint16_t hw_adc_read_raw(int adc_csn_pin, int channel) {

    uint16_t adc_value = read_mcp3208(channel, adc_csn_pin);
    
    if (adc_value == 4095)
        adc_value = 0; // error reading

    return adc_value;

}

#endif


#ifndef USE_HW_OPTICAL
uint16_t hw_adc_read_raw(int adc_index, int channel) {
    
    for (int i = 0; i < NUM_TUBES - 6; i++) {                                                                      
        gOpticalInputs.intensity[i] = (OPTICAL_REACTION_THRESHOLD + 100);
    }
    for (int i = NUM_TUBES - 6; i < NUM_TUBES; i++) {
        gOpticalInputs.intensity[i] = 200;
    }
    return 0;
}

#endif
