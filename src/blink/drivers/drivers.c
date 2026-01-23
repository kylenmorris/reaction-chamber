#include "button_interface.h"
#include "data_structs.h"
#include "heater_interface.h"
#include "temp_sens_interface.h"
#include "tube_optical_interface.h"
#include "tube_sens_interface.h"
#include <stdlib.h>

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
    #ifdef USE_HW_BUTTONS
        return gpio_get(button_gpio);
    #else
        return 1;
    #endif
}


void hw_heater_toggle(bool enable) {
    #ifdef USE_HW_HEATER
        if (enable) {
            gpio_put(HEATER_PIN, 1);
        } else {
            gpio_put(HEATER_PIN, 0);
        }
    #else
        
    #endif
}


float hw_read_temperature_sensor(int sensor_id) {
    #ifdef USE_HW_TEMP_SENS

    #else
        if (gTempStatus.chamber_temp < TEMP_LOW_C + 1.5) {
            return gTempStatus.chamber_temp + 0.1f;
        } else {
            return gTempStatus.chamber_temp;
        }
    #endif
}


uint16_t hw_adc_read_raw(int adc_index, int channel) {
    #ifdef USE_HW_OPTICAL

    #else
        for (int i = 0; i < NUM_TUBES - 6; i++) {                                                                      
            gOpticalInputs.intensity[i] = (OPTICAL_REACTION_THRESHOLD + 100 - rand() % 100000);
        }
        for (int i = NUM_TUBES - 6; i < NUM_TUBES; i++) {
            gOpticalInputs.intensity[i] = rand() % 3000;
        }
    #endif
}

// Track which tubes have been inserted via debug button
static bool used_tubes[NUM_TUBES] = { false };

uint16_t hw_tube_sens_read_all(void) {
    #ifdef USE_HW_TUBE_SENS

    #else
        // Handle debug button press to insert new random tube
        if (button_is_pressed(DEBUG)) {

            // printf("Debug button pressed, inserting tube\n");
            // Find an unused tube
            int available_tubes[NUM_TUBES];
            int available_count = 0;
            
            for (int i = 0; i < NUM_TUBES; i++) {
                if (!used_tubes[i]) {
                    available_tubes[available_count++] = i;
                }
            }
            
            // If tubes are available, pick a random one
            if (available_count > 0) {
                int random_idx = rand() % available_count;
                int tube_index = available_tubes[random_idx];

                // printf("Inserting tube %d\n", tube_index);
                
                gSysControl.tube_present[tube_index] = true;
                used_tubes[tube_index] = true;
            }
        }
    #endif
}
