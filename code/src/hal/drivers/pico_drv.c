#include "drivers.h"
#include "constants.h"

#ifndef USE_HW_PICO

#include "data_structs.h"
#include "stdio.h"

// Turn the led on or off
void pico_set_led(bool led_on) {
    printf("LED %s\n", led_on ? "ON" : "OFF");
}

uint32_t get_current_time(void) {
    gSimTime += 10;
    return gSimTime;
}

#endif

#ifdef USE_HW_PICO

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <pico/time.h>

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