/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "include/data_structs.h"
#include "pico/stdlib.h"
#include "constants.h"

#include "include/button_ctrl.h"
#include <hardware/gpio.h>
#include <stdbool.h>
#include <stdio.h>

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

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

int main() {
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    stdio_init_all();

    printf("Starting up...\n");
    pico_set_led(HIGH);
    button_init();

    sleep_ms(500);
    pico_set_led(LOW);

    while (true) {

        button_step();
        
        // this would be the state update or something instead
        if (gButtonInput.wasPressed) {
            char buffer[100];
            sprintf(buffer, "Button pressed on GPIO pin: %d\n", gButtonInput.lastPressed);
            printf("%s", buffer);
            gButtonInput.wasPressed = false;
        } 

        sleep_ms(SYSTEM_DELAY_MS);
    }
}


