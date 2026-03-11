#include "drivers.h"

#include "data_helpers.h"
#include "data_structs.h"
#include "constants.h"
#include <hardware/i2c.h>
#include <hardware/spi.h>
#include <stdint.h>
#include <stdlib.h>

#include <hardware/gpio.h>
#include "pico/time.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifdef USE_HW_PICO

// Turn the led on or off
void pico_set_led(bool led_on) {
    #if defined(PICO_DEFAULT_LED_PIN)
        // Just set the GPIO on or off
        gpio_put(PICO_DEFAULT_LED_PIN, led_on);
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        // Ask the wifi "driver" to set the GPIO on or off
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
}

uint32_t get_current_time(void) {
    return to_ms_since_boot(get_absolute_time());
}

#endif

#ifdef USE_HW_BUTTONS
int hw_button_get_raw(int button_gpio) {
    return gpio_get(button_gpio);
}
#endif

#ifdef USE_HW_HEATER
void hw_heater_toggle(bool enable) {
    if (enable) {
        gpio_put(HEATER_PIN, 1);
        // printf("Heater pin ON\n");
    } else {
        gpio_put(HEATER_PIN, 0);
        // printf("Heater pin OFF\n");
    }
}
#endif

#ifdef USE_HW_TEMP_SENS

// adapted from 
// https://github.com/raspberrypi/pico-examples/blob/master/i2c/mcp9808_i2c/mcp9808_i2c.c
// into gemini
float mcp9808_convert_temp(uint8_t upper_byte, uint8_t lower_byte) {
    float temperature;

    // Mask the flags (Bits 15, 14, 13) out. 
    // 0x1F keeps only the Sign bit (bit 12) and the 4 integer bits.
    upper_byte = upper_byte & 0x1F; 

    // Check the SIGN bit (Bit 12 / 0x10)
    if ((upper_byte & 0x10) == 0x10) {
        // Negative Temperature logic
        upper_byte = upper_byte & 0x0F; // Clear sign bit for math
        temperature = 256.0 - (((float)upper_byte * 16.0) + ((float)lower_byte / 16.0));
    } else {
        // Positive Temperature logic
        // Mask upper_byte to 0x0F to ensure no flags interfere with math
        temperature = (((float)upper_byte) * 16.0) + ((float)lower_byte / 16.0);
    }

    return temperature;
}

float hw_read_temperature_sensor(int address) {

    // printf("Reading temperature sensor at address 0x%02X\n", address);
    const uint8_t reg = 0x05;
    uint8_t buf[2];
    
    int w = i2c_write_timeout_us(i2c0, address, &reg, 1, true, 50000);
    int r = i2c_read_timeout_us(i2c0, address, buf, 2, false, 50000);

    if ((r < 0) || (w < 0)) {
        gSystemInfo.temp_sensor_success_count--;
        if (gSystemInfo.temp_sensor_success_count < -5) {
            gSystemError.current_error = ERROR_TEMP_SENSOR_FAULT;
            gSystemInfo.temp_sensor_success_count = -6;
        }
        return -999;
    } else {
        gSystemInfo.temp_sensor_success_count++;
    }

    float temp = mcp9808_convert_temp(buf[0], buf[1]);

    // uint16_t raw = ((uint16_t)buf[0] << 8) | buf[1];
    // raw &= 0x1FFF;

    // float temp = raw * 0.0625f;

    // if (raw & 0x1000)
    //     temp -= 256.0f;

    if (temp < -40.0f || temp > 125.0f) {
        // Out of range, likely a faulty reading
        gSystemInfo.temp_sensor_success_count--;
        if (gSystemInfo.temp_sensor_success_count < -5) {
            gSystemError.current_error = ERROR_TEMP_SENSOR_FAULT;
            gSystemInfo.temp_sensor_success_count = -6;
        }
        return -999;
    }

    return temp;
}

#endif

#ifdef USE_HW_OPTICAL

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

#ifdef USE_HW_TUBE_SENS

uint8_t hw_tube_sens_read_all(int pin) {
    uint8_t data;

    // Disable clock (CLKINH = 1)
    gpio_put(pin, 1);

    // Take MOSI away from SPI as I screwed up the pcb, plz forgive me
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(SPI1_MOSI_PIN, GPIO_OUT);

    // Parallel load
    gpio_put(SPI1_MOSI_PIN, 0);
    sleep_us(1);
    gpio_put(SPI1_MOSI_PIN, 1);
    sleep_us(1);

    // Give MOSI back to SPI
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SPI);

    // Enable clock
    gpio_put(pin, 0);

    // Clock 8 bits
    spi_read_blocking(SPI_INSTANCE_SW, 0xFF, &data, 1);

    // Disable clock again
    gpio_put(pin, 1);

    return ~data;   // because QH* is inverted output
}

#endif
