#include "button_interface.h"
#include "heater_interface.h"
#include "temp_sens_interface.h"
#include "tube_optical_interface.h"
#include "tube_sens_interface.h"

#include "constants.h"
#include "led_interface.h"
#include <hardware/gpio.h>

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

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

// Checks for button presses and updates gButtonInput if a button was pressed
int hw_button_get_raw(int button_gpio) {
    return gpio_get(button_gpio);
}


void hw_heater_toggle(bool enable) {
    if (enable) {
        gpio_put(HEATER_PIN, 1);
    } else {
        gpio_put(HEATER_PIN, 0);
    }
}


// float hw_read_temperature_sensor(int sensor_id) {

// }


// uint16_t hw_adc_read_raw(int adc_index, int channel) {

// }


// void hw_led_set(bool led_on) {

// }


// uint16_t hw_tube_sens_read_all(void) {

// }
