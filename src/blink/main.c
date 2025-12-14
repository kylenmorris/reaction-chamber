/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "imodel_structs.h"
#include "include/data_structs.h"
#include "pico/stdlib.h"
#include "constants.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"

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

SystemState update_state(SystemState current_state) {
    
    button_step();  
    temp_sens_step();
    draw_display(current_state);

    switch (current_state) {
        case IDLE:

            // no inputs means stay in idle
            if (!gButtonInput.wasPressed) {
                break;
            }

            if (gButtonInput.lastPressed == UP) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuModel.selected_index = 0;
            }
            
            if (gButtonInput.lastPressed == DOWN) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuModel.selected_index = 1;
            }
            
            if (gButtonInput.lastPressed == SELECT) {
                gButtonInput.wasPressed = false;  // Reset flag

                if (gIdleMenuModel.selected_index == 0) {
                    heater_init();
                    temp_sens_init();

                    return HEATING;
                }
                else if (gIdleMenuModel.selected_index == 1) {
                    return HISTORY;
                }
            }

            // back does nothing in idle

            break;

        case HEATING:

            if (gButtonInput.wasPressed && gButtonInput.lastPressed == BACK) {
                gButtonInput.wasPressed = false;  // Reset flag
                return IDLE;
            }

            if (gTempStatus.chamber_temp >= gTempStatus.target_temp) { // this should probably be a flag in gTempStatus
                return REACTING;
            }
            
            break;

        case REACTING:
            return RESULTS;

        case RESULTS:
            return IDLE;

        case HISTORY:
            return IDLE;
    }
    return current_state; // No change if no conditions met
}

int main() {
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    stdio_init_all();

    printf("Starting up...\n");
    pico_set_led(HIGH);

    button_init();
    temp_sens_init();
    heater_init();
    tube_optical_init();

    SystemState current_state = IDLE;

    sleep_ms(500);
    printf("Finished.\n");
    pico_set_led(LOW);

    while (true) {      

        current_state = update_state(current_state);
        sleep_ms(SYSTEM_DELAY_MS);

    }
}

