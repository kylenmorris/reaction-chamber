#include "drivers.h"
#include "data_structs.h"
#include "constants.h"
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
float hw_read_temperature_sensor(int sensor_id) {

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
