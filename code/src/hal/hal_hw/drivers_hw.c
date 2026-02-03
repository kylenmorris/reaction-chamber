#include "drivers.h"
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
    } else {
        gpio_put(HEATER_PIN, 0);
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

float hw_read_temperature_sensor(int sensor_id) {

    const uint8_t REG_TEMP_AMB = 0x05;

    uint8_t buf[2];
    uint16_t upper_byte;
    uint16_t lower_byte;

    float temperature;

    i2c_write_blocking(i2c1, TEMP_SENSOR_0_I2C_ADDRESS, &REG_TEMP_AMB, 1, true);
    i2c_read_blocking(i2c1, TEMP_SENSOR_0_I2C_ADDRESS, buf, 2, false);

    upper_byte = buf[0];
    lower_byte = buf[1];
    printf("Raw: {%d} {%d}\n", upper_byte, lower_byte);
    //clears flag bits in upper byte
    temperature = mcp9808_convert_temp(upper_byte & 0xF, lower_byte & 0xF0);
    
    printf("Ambient temperature: %.4f°C\n", temperature);
    
    return temperature;
}
#endif

#ifdef USE_HW_OPTICAL
uint16_t hw_adc_read_raw(int adc_index, int channel) {
    
}
#endif

// Track which tubes have been inserted via debug button
static bool used_tubes[NUM_TUBES] = { false };

#ifdef USE_HW_TUBE_SENS
uint16_t hw_tube_sens_read_all(void) {
    uint8_t data;
    
    gpio_put(SPI1_CS_SHIFT_SWITCHES_SR_PIN, 0);
    sleep_us(100); // Could be as short as 100ns but I see no need for that
    gpio_put(SPI1_CS_SHIFT_SWITCHES_SR_PIN, 1);

    spi_read_blocking(SPI_INSTANCE_SW, 0x0, &data, 1);

    return data;
}
#endif
