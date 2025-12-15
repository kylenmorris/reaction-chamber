
#include <hardware/gpio.h>
#include <stdbool.h>
#include <stdio.h>
#include "model/data_structs.h"
#include "pico/stdlib.h" // this IS needed despite clangd saying otherwise
#include "pico/multicore.h"

#include "imodel_structs.h"
#include "data_structs.h"
#include "data_structs.h"
#include "constants.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"

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

// may move this to helper file it's going to get long
void update_state() {
    
 
    temp_sens_ctrl_step();

    switch (gSystemState) {
        case IDLE:

            // no inputs means stay in idle
            if (!gButtonInput.wasPressed) {
                break;
            }

            gIdleMenuIM.needs_redraw = true;

            if (gButtonInput.lastPressed == UP) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 0;
            }
            
            if (gButtonInput.lastPressed == DOWN) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 1;
            }
            
            if (gButtonInput.lastPressed == SELECT) {
                gButtonInput.wasPressed = false;  // Reset flag

                if (gIdleMenuIM.selected_index == 0) {
                    heater_ctrl_init();
                    temp_sens_ctrl_init();
                    
                    gHeatingMenuIM.needs_redraw = true;
                    gSystemState = HEATING;
                }
                else if (gIdleMenuIM.selected_index == 1) {
                    gSystemState = HISTORY;
                }
            }

            // back does nothing in idle

            break;

        case HEATING:

            if (gButtonInput.wasPressed && gButtonInput.lastPressed == BACK) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            if (gTempStatus.chamber_temp >= gTempStatus.target_temp) { // this should probably be a flag in gTempStatus
                gSystemState = REACTING;
            }

            break;

        case REACTING:
            gSystemState = RESULTS;

        case RESULTS:
            gSystemState = IDLE;

        case HISTORY:
            gSystemState = IDLE;
    }

}

void core1_entry() {
    while (true) {
        button_ctrl_step(); 
        display_ctrl_draw();
        sleep_ms(SYSTEM_DELAY_MS);
    }
}

int main() {

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    pico_set_led(HIGH); // Startup LED

    stdio_init_all();

    button_ctrl_init();
    temp_sens_ctrl_init();
    heater_ctrl_init();
    tube_optical_ctrl_init();

    multicore_launch_core1(core1_entry);

    // SystemState current_state = IDLE;

    sleep_ms(500);
    pico_set_led(LOW); // Finished startup

    while (true) {      

        update_state();
        sleep_ms(SYSTEM_DELAY_MS);

    }
}

